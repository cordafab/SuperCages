#include "meanValueCoordinates.h"

#include "external/JM/point3.h"
#include "external/JM/MVCoordinates3D.h"

MeanValueCoordinates::MeanValueCoordinates()
{

}

MeanValueCoordinates::MeanValueCoordinates(Weights   * _w,
                                           Character * _character,
                                           Cage      * _cage)
   :CageSkinning(_character, _cage)
{
   weights = _w;
}

MeanValueCoordinates::MeanValueCoordinates(Character * _character,
                                           Cage      * _cage)
   :CageSkinning(_character, _cage)
{
   generateCoords(weights, character, cage);
}

MeanValueCoordinates::~MeanValueCoordinates()
{

}

bool MeanValueCoordinates::create(Weights *_w, Character *_character, Cage *_cage)
{
   clear();

   CageSkinning::create(_character, _cage);

   weights = _w;

   return true;

}

bool MeanValueCoordinates::create(Character *_character, Cage *_cage)
{
   clear();

   CageSkinning::create(_character, _cage);

   generateCoords(weights, character, cage);

   return true;
}

void MeanValueCoordinates::clear()
{
   CageSkinning::clear();

   weights = nullptr;
}

void MeanValueCoordinates::deform()
{
   const ulong vertexNumber = character->getNumVertices();
   //const ulong handleNumber = cage->getNumVertices();
   const std::vector<double> & cageVertices = cage->getCurrentPoseVertices();
   const std::vector<double> & restCageVertices = cage->getRestPoseVertices();
   std::vector<double> & charVertices = character->getActualPoseVerticesVector();
   std::vector<double> & restCharVertices = character->getRestPoseVerticesVector();

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<vertexNumber; ++i)
   {
      double vRestx = 0.0;
      double vResty = 0.0;
      double vRestz = 0.0;

      double vx = 0.0;
      double vy = 0.0;
      double vz = 0.0;

      const std::vector<ulong> & handles = weights->getNonZeros(i);
      const std::vector<double> & vWeights = weights->getWeights(i);

      for(ulong j=0; j<handles.size(); ++j)
      {
         const ulong h = handles[j];
         const double wh = vWeights[h];

         const double oldVRestx = restCageVertices[h*3+0];
         const double oldVResty = restCageVertices[h*3+1];
         const double oldVRestz = restCageVertices[h*3+2];

         const double oldVx = cageVertices[h*3+0];
         const double oldVy = cageVertices[h*3+1];
         const double oldVz = cageVertices[h*3+2];

         vRestx += wh * oldVRestx;
         vResty += wh * oldVResty;
         vRestz += wh * oldVRestz;

         vx += wh * oldVx;
         vy += wh * oldVy;
         vz += wh * oldVz;
      }

      restCharVertices[i*3+0] = vRestx;
      restCharVertices[i*3+1] = vResty;
      restCharVertices[i*3+2] = vRestz;

      charVertices[i*3+0] = vx;
      charVertices[i*3+1] = vy;
      charVertices[i*3+2] = vz;
   }
}

bool MeanValueCoordinates::generateCoords(Weights   * & weights,
                                          Character *   character,
                                          Cage      *   cage)
{
   std::vector< std::vector< double > > vecWeights;
   vecWeights.resize( character->getNumVertices() );

   std::vector< jm::point3d > cage_vertices( cage->getNumVertices() );
   std::vector< std::vector< int > > cage_triangles( cage->getNumTriangles() );
   const std::vector<double> & cg3Vertices = cage->getOriginalRestPoseVertices();
   const std::vector<int> & cg3Tris = cage->getOriginalRestPoseTriangles();

   for( unsigned long v = 0 ; v < cage->getNumVertices() ; ++v )
   {
      jm::point3d p(cg3Vertices[v*3+0],cg3Vertices[v*3+1],cg3Vertices[v*3+2]);
      cage_vertices[v] = p;
   }

   for(ulong t = 0 ; t < cage->getNumTriangles() ; ++t )
   {
      std::vector<int> tv(3);
      tv[0] = cg3Tris[t*3+0];
      tv[1] = cg3Tris[t*3+1];
      tv[2] = cg3Tris[t*3+2];
      cage_triangles[t] = tv;
   }

   std::vector< jm::point3d > char_vertices( character->getNumVertices() );
   const std::vector<double> & cg3char_vertices = character->getRestPoseVerticesVector();

   for( unsigned long v = 0 ; v < character->getNumVertices() ; ++v )
   {
      jm::point3d p(cg3char_vertices[v*3+0],cg3char_vertices[v*3+1],cg3char_vertices[v*3+2]);
      char_vertices[v] = p;
   }

   for(ulong i=0; i<character->getNumVertices(); ++i)
   {
      vecWeights[i].resize(cage->getNumVertices());

      jm::point3d jmCharVert(
               cg3char_vertices[i*3+0],
            cg3char_vertices[i*3+1],
            cg3char_vertices[i*3+2]);

      std::vector< double > mvcoords( character->getNumVertices() , 0.0 );

      MVCoordinates::MVC3D::computeCoordinates( jmCharVert ,
                                                cage_triangles ,
                                                cage_vertices ,
                                                vecWeights[i] );
   }

   //Transfering weights from vecWeights to Weights
   weights = new Weights(character->getNumVertices(),cage->getNumVertices());
   for(unsigned long i=0; i<vecWeights.size(); ++i)
   {
      for(unsigned long j=0; j<vecWeights[i].size(); ++j)
      {
         weights->setWeight(j,i,vecWeights[i][j]);
      }
   }
   return true;
}

Weights * MeanValueCoordinates::getWeights()
{
   return weights;
}
