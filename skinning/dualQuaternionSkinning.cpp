#include "dualQuaternionSkinning.h"
#include "math/dualQuaternion.h"

DualQuaternionSkinning::DualQuaternionSkinning()
{

}

DualQuaternionSkinning::DualQuaternionSkinning(Weights *_w,
                                               Character *_character,
                                               Skeleton *_skeleton)
{
   create(_w,_character, _skeleton);
}

bool DualQuaternionSkinning::create(Weights *_w, Character *_character, Skeleton *_skeleton)
{
   clear();

   SkeletonSkinning::create(_w,_character, _skeleton);

   return true;
}

void DualQuaternionSkinning::clear()
{
   SkeletonSkinning::clear();
}

void DualQuaternionSkinning::deform()
{
   ulong vertexNumber = character->getNumVertices();


   std::vector<cg3::dDualQuaternion> qList;
   const std::vector<SkeletonNode> & skelNodes = skeleton->getNodesVector();

   for(ulong h=0; h<skeleton->getNumNodes(); ++h)
   {
      const cg3::Transform & Th = skelNodes[h].getGlobalT();
      qList.push_back(cg3::dDualQuaternion(Th));
   }

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<vertexNumber; ++i)
   {
      cg3::dDualQuaternion q;
      q.setZero();
      const std::vector<ulong> & handles = w->getNonZeros(i);
      const std::vector<double> & vWeights = w->getWeights(i);

      for(unsigned int j=0; j<handles.size(); ++j)
      {
         const ulong h0 = handles[0];
         const ulong h = handles[j];
         const double wh = vWeights[h];
         const double v = std::copysign(1.0,qList[h0].dot(qList[h]));
         q = q + (wh * v) * qList[h];
      }

      q = q.normalize();

      cg3::Point3d v = character->getRestPoseVertex(i);
      cg3::Point3d v1 = q.applyTransformation(v);

      if(rootMotion) v1 = skeleton->getRootMotion().applyToPoint(v1);
      character->setVertex(i, v1);
      //character->setSkelPoseVertex(i, v1);



   }
}
