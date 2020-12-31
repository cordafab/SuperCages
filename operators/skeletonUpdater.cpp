#include "skeletonUpdater.h"

#include "_external/JM/point3.h"
#include "_external/JM/MEC.h"
#include "_external/JM/MVCoordinates3D.h"

SkeletonUpdater::SkeletonUpdater()
{

}

SkeletonUpdater::SkeletonUpdater(Weights   *_wSkeleton,
                                 Weights   *_wCage,
                                 Character *_character,
                                 Skeleton  *_skeleton,
                                 Cage      *_cage)
{

   create(_wSkeleton,
          _wCage,
          _character,
          _skeleton,
          _cage);
}

bool SkeletonUpdater::create(Weights *_wSkeleton,
                             Weights *_wCage,
                             Character *_character,
                             Skeleton *_skeleton,
                             Cage *_cage)
{
   clear();

   setSkeletonWeights(_wSkeleton);
   setCageWeights(_wCage);
   setCharacter(_character);
   setSkeleton(_skeleton);
   setCage(_cage);

   generateSkeletonUpdaterWeights(skeletonUpdaterWeights,
                                  wSkel,
                                  wCage,
                                  character,
                                  skeleton,
                                  cage);

   originalNodePositions.resize(skeleton->getNumNodes());

   for( unsigned long i = 0 ; i < skeleton->getNumNodes() ; ++i )
   {
      originalNodePositions[i] = skeleton->getNode(i).getLocalTCurrent().getTranslation();
   }

   return true;
}

void SkeletonUpdater::clear()
{
   wSkel = nullptr;
   wCage = nullptr;
   character = nullptr;
   skeleton = nullptr;
   cage = nullptr;

   skeletonUpdaterWeights = nullptr;;

   originalNodePositions.clear();
}

bool SkeletonUpdater::generateSkeletonUpdaterWeights(
      Weights   * & skeletonUpdaterWeights,
      Weights   * skeletonWeights,
      Weights   * cageWeights,
      Character * character,
      Skeleton  * skeleton,
      Cage      * cage)
{

   std::cout << "\t find_weights_for_articulations" << std::endl;
   std::vector< std::vector< double > > AVWeights;
   AVWeights.resize( skeleton->getNumNodes() );
   std::vector< jm::point3d > mesh_vertices( character->getNumVertices() );
   std::vector< std::vector< int > > mesh_triangles( character->getNumTriangles() );
   const std::vector<double> & cg3Vertices = character->getRestPoseVerticesVector();
   const std::vector<int> & cg3Tris = character->getTrianglesVector();


   #pragma omp parallel for schedule(static)
   for( unsigned long v = 0 ; v < character->getNumVertices() ; ++v )
   {
      jm::point3d p(cg3Vertices[v*3+0],cg3Vertices[v*3+1],cg3Vertices[v*3+2]);
      mesh_vertices[v] = p;
   }


   #pragma omp parallel for schedule(static)
   for( unsigned long t = 0 ; t < character->getNumTriangles() ; ++t )
   {
      std::vector<int> tv(3);
      tv[0] = cg3Tris[t*3+0];
      tv[1] = cg3Tris[t*3+1];
      tv[2] = cg3Tris[t*3+2];
      mesh_triangles[t] = tv;
   }

   std::vector< jm::point3d > cage_vertices( cage->getNumVertices() );
   const std::vector<double> & cg3cage_vertices = cage->getRestPoseVertices();


   #pragma omp parallel for schedule(static)
   for( unsigned long v = 0 ; v < cage->getNumVertices() ; ++v )
   {
      jm::point3d p(cg3cage_vertices[v*3+0],cg3cage_vertices[v*3+1],cg3cage_vertices[v*3+2]);
      cage_vertices[v] = p;
   }


   skeletonUpdaterWeights = new Weights(skeleton->getNumNodes(),cage->getNumVertices());


   double sExponent = 0.05; // jm: fine-tuned this parameter
   #pragma omp parallel for schedule(static)
   for( unsigned long j = 0 ; j < skeleton->getNumNodes() ; ++j )
   {
      double epsilon = 0.01;

      AVWeights[j].resize( character->getNumVertices() );
      std::vector<int> bonesAdj;
      int father = skeleton->getNode(j).getFather();
      if(father!=-1)
         bonesAdj.push_back(father);
      bonesAdj.push_back(j);

      cg3::Point3d cg3JointJ = skeleton->getNode(j).getGlobalTRest().getTranslation();
      jm::point3d jmJointJ(cg3JointJ[0],cg3JointJ[1],cg3JointJ[2]);

      std::vector< double > mvcoords( character->getNumVertices() , 0.0 );
      std::vector< double > weightPrior( character->getNumVertices() , 0.0 );
      //initialize weightPrior with the MVC of p with respect to the character
      MVCoordinates::MVC3D::computeCoordinates( jmJointJ ,
                                                mesh_triangles ,
                                                mesh_vertices ,
                                                mvcoords );
      double sumPriors = 0.0;

      // then
      for( unsigned long v = 0 ; v < character->getNumVertices() ; ++v ) {
         double localityFactor = -1;
         for(int b:bonesAdj)
         {
            double w_vb = skeletonWeights->getWeight(b,v);
            {
               localityFactor += pow(std::max<double>(epsilon,fabs(w_vb)) , sExponent); // jm: add max(epsilon, . ) to handle 0 weights joints ("fake" joints)
            }
         }
         localityFactor = std::max<double>(localityFactor , epsilon);
         weightPrior[v] = mvcoords[v] * localityFactor;
         //TO DO: Try to undertstand why this can fail
         sumPriors += weightPrior[v];
      }

      {
         //Express the skeleton nodes as a linear combination of the cage vertices:
         std::vector< double > jointWeightsInvalid( cage->getNumVertices() , 0.0 );
         for(ulong c = 0; c < cage->getNumVertices(); ++c)
         {
            for(ulong v = 0; v < character->getNumVertices(); ++v)
            {
               jointWeightsInvalid[c] += weightPrior[v] * cageWeights->getWeight(c,v);
            }
         }

         //TEST
         std::vector< double > charSkelWeights( character->getNumVertices() , 0.0 );
         MEC::computeCoordinates< jm::mat33d >( jmJointJ , mesh_vertices , weightPrior , charSkelWeights , 100 , 50 , 0.001 );
         //END TEST

         std::vector< double > jointWeights( cage->getNumVertices() , 0.0 );
         MEC::Stats mecStats = MEC::computeCoordinates< jm::mat33d >( jmJointJ , cage_vertices , jointWeightsInvalid , jointWeights , 100 , 50 , 0.001 );

         // AWFUL safe guard:
         if( mecStats.linearPrecisionError > 0.000001 ) {
            std::cout << "MEC hass failed with the LBS derived prior. Computation without it" << std::endl;
            jointWeightsInvalid.clear();
            jointWeightsInvalid.resize(cage->getNumVertices() , 0.0 );
            for(ulong c = 0; c < cage->getNumVertices(); ++c)
            {
               for(ulong v = 0; v < character->getNumVertices(); ++v)
               {
                  jointWeightsInvalid[c] += mvcoords[v] * cageWeights->getWeight(c,v);
               }
            }
            mecStats = MEC::computeCoordinates< jm::mat33d >( jmJointJ , cage_vertices , jointWeightsInvalid , jointWeights , 100 , 50 , 0.001 );
         }

         for(ulong c = 0; c < cage->getNumVertices(); ++c)
         {
            skeletonUpdaterWeights->setWeight(c,j,jointWeights[c]);
         }
      }
   }
   std::cout << "\t find_weights_for_articulations done" <<std::endl;

   return true;
}


void SkeletonUpdater::updatePosition()
{
   const std::vector<double> & cageVerticesRest = cage->getRestPoseVertices();
   std::vector<SkeletonNode> & skeletonNodes = skeleton->getNodesVector();
   cg3::Vec3d rootMotion;

   //#pragma omp parallel for schedule(static)
   for(unsigned long j=0; j<skeletonNodes.size(); ++j)
   {
      if(skeletonNodes[j].getFather()==-1)
      {

         rootMotion = skeletonNodes[j].getLocalTCurrent().getTranslation() - skeletonNodes[j].getLocalTRest().getTranslation();
      }

      cg3::Point3d pRest;
      for( unsigned long c = 0 ; c < cage->getNumVertices() ; ++c )
      {
         double w = skeletonUpdaterWeights->getWeight(c,j);
         pRest[0] += w * cageVerticesRest[c*3+0];
         pRest[1] += w * cageVerticesRest[c*3+1];
         pRest[2] += w * cageVerticesRest[c*3+2];
      }

      skeletonNodes[j].getGlobalTRest().setTranslation(pRest);
   }

   skeleton->updateLocalFromGlobalRest();

   for(unsigned long j=0; j<skeletonNodes.size() ; ++j)
   {
      cg3::Vec3d t = skeletonNodes[j].getLocalTRest().getTranslation();
      if(skeletonNodes[j].getFather()==-1)
      {
         t += rootMotion;
      }
      skeletonNodes[j].getLocalTCurrent().setTranslation(t);
   }

   skeleton->updateGlobalFromLocalCurrent();
}

bool SkeletonUpdater::generateSkeletonUpdaterWeights(Weights   * & skeletonUpdaterWeights,
                                                     Weights   *   skeletonWeights,
                                                     Character *   character,
                                                     Skeleton  *   skeleton)
{
   std::vector< std::vector< double > > AVWeights;
   AVWeights.resize( skeleton->getNumNodes() );
   std::vector< jm::point3d > mesh_vertices( character->getNumVertices() );
   std::vector< std::vector< int > > mesh_triangles( character->getNumTriangles() );
   const std::vector<double> & cg3Vertices = character->getRestPoseVerticesVector();
   const std::vector<int> & cg3Tris = character->getTrianglesVector();


   #pragma omp parallel for schedule(static)
   for( unsigned long v = 0 ; v < character->getNumVertices() ; ++v )
   {
      jm::point3d p(cg3Vertices[v*3+0],cg3Vertices[v*3+1],cg3Vertices[v*3+2]);
      mesh_vertices[v] = p;
   }


   #pragma omp parallel for schedule(static)
   for( unsigned long t = 0 ; t < character->getNumTriangles() ; ++t )
   {
      std::vector<int> tv(3);
      tv[0] = cg3Tris[t*3+0];
      tv[1] = cg3Tris[t*3+1];
      tv[2] = cg3Tris[t*3+2];
      mesh_triangles[t] = tv;
   }

   skeletonUpdaterWeights = new Weights(skeleton->getNumNodes(),character->getNumVertices());


   double sExponent = 0.05; // jm: fine-tuned this parameter
   for( unsigned long j = 0 ; j < skeleton->getNumNodes() ; ++j )
   {
      double epsilon = 0.01;

      AVWeights[j].resize( character->getNumVertices() );
      std::vector<int> bonesAdj;
      int father = skeleton->getNode(j).getFather();
      if(father!=-1)
         bonesAdj.push_back(father);
      bonesAdj.push_back(j);

      cg3::Point3d cg3JointJ = skeleton->getNode(j).getGlobalTRest().getTranslation();
      jm::point3d jmJointJ(cg3JointJ[0],cg3JointJ[1],cg3JointJ[2]);

      std::vector< double > mvcoords( character->getNumVertices() , 0.0 );
      std::vector< double > weightPrior( character->getNumVertices() , 0.0 );
      //initialize weightPrior with the MVC of p with respect to the character
      MVCoordinates::MVC3D::computeCoordinates( jmJointJ ,
                                                mesh_triangles ,
                                                mesh_vertices ,
                                                mvcoords );
      double sumPriors = 0.0;

      // then
      for( unsigned long v = 0 ; v < character->getNumVertices() ; ++v ) {
         double localityFactor = -1;
         for(int b:bonesAdj)
         {
            double w_vb = skeletonWeights->getWeight(b,v);
            {
               localityFactor += pow(std::max<double>(epsilon,fabs(w_vb)) , sExponent); // jm: add max(epsilon, . ) to handle 0 weights joints ("fake" joints)
            }
         }
         localityFactor = std::max<double>(localityFactor , epsilon);
         weightPrior[v] = mvcoords[v] * localityFactor;
         sumPriors += weightPrior[v];
      }

      {
         std::vector< double > charSkelWeights( character->getNumVertices() , 0.0 );
         MEC::computeCoordinates< jm::mat33d >( jmJointJ , mesh_vertices , weightPrior , charSkelWeights , 100 , 50 , 0.001 );
         for(ulong v = 0; v < character->getNumVertices(); ++v)
         {
            skeletonUpdaterWeights->setWeight(v,j,charSkelWeights[v]);
         }
      }
   }
   return true;
}
