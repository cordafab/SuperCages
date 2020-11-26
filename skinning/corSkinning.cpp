#include "corSkinning.h"

CoRSkinning::CoRSkinning()
{

}

CoRSkinning::CoRSkinning(Weights   *_w,
                         Character *_character,
                         Cage      *_cage,
                         Skeleton  *_skeleton,
                         Weights   *_corWeights)
{
   create(_w, _character, _cage, _skeleton, _corWeights);
}

bool CoRSkinning::create(Weights *_w,
                         Character *_character,
                         Cage *_cage,
                         Skeleton *_skeleton,
                         Weights *_corWeights)
{
   clear();

   SkeletonSkinning::create(_w,_character, _skeleton);
   cage = _cage;

   corWeights = _corWeights;
   cor.resize(character->getNumVertices());
   updateCoRs();

   return true;
}

void CoRSkinning::clear()
{
   SkeletonSkinning::clear();

   corWeights = nullptr;
   cage       = nullptr;
   cor.clear();
}

void CoRSkinning::updateCoRs()
{
   const std::vector<double> & vertices = cage->getRestPoseVerticesVector();

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<character->getNumVertices(); ++i)
   {
      double cx = 0.0;
      double cy = 0.0;
      double cz = 0.0;

      const std::vector<double> & vecCorWeights = corWeights->getWeights(i);

      for(ulong j=0; j<cage->getNumVertices(); ++j)
      {
         double w = vecCorWeights[j];
         cx += w * vertices[j*3+0];
         cy += w * vertices[j*3+1];
         cz += w * vertices[j*3+2];
      }

      cor[i][0] = cx;
      cor[i][1] = cy;
      cor[i][2] = cz;
   }
}

void CoRSkinning::deform()
{
   updateCoRs();
   ulong vertexNumber = character->getNumVertices();

   const std::vector<SkeletonNode> & skelNodes = skeleton->getNodesVector();
   std::vector<cg3::dQuaternion> qList;

   for(ulong h=0; h<skeleton->getNumNodes(); ++h)
   {
      const cg3::Transform & Th = skelNodes[h].getGlobalT();
      qList.push_back(Th.getRotation());
   }

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<vertexNumber; ++i)
   {
      cg3::dQuaternion q;
      q.setZero();
      const std::vector<ulong> & handles = w->getNonZeros(i);
      const std::vector<double> & vWeights = w->getWeights(i);

      for(ulong j=0; j<handles.size(); ++j)
      {
         const ulong h0 = handles[0];
         const ulong h = handles[j];
         const double wh = vWeights[h];
         const double v = std::copysign(1.0,qList[h0].dot(qList[h]));
         q = q + (wh * v) * qList[h];
      }

      q.normalize();
      cg3::Transform R(q); //OK

      // Transform the cor with LBS:
      cg3::Point3d cor_transformed;
      for(ulong h : handles)
      {
         const cg3::Transform & Th = skelNodes[h].getGlobalT();
         cor_transformed += vWeights[h] * Th.applyToPoint(cor[i]);
      }

      cg3::Transform Tfinal( q );
      Tfinal.setTranslation( cor_transformed  - q.applyRotation(cor[i]) );

      // apply final transformation:
      cg3::Point3d v = character->getRestPoseVertex(i);
      cg3::Point3d v1 = Tfinal.applyToPoint(v);

      if(rootMotion) v1 = skeleton->getRootMotion().applyToPoint(v1);
      character->setVertex(i, v1);
      //character->setSkelPoseVertex(i, v1);

   }
}
