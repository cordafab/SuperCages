#include "linearBlendSkinning.h"

LinearBlendSkinning::LinearBlendSkinning()
{

}


LinearBlendSkinning::LinearBlendSkinning(Weights *_w, Character *_character, Skeleton *_skeleton)
{
   create(_w,_character, _skeleton);
}

bool LinearBlendSkinning::create(Weights *_w, Character *_character, Skeleton *_skeleton)
{
   clear();

   SkeletonSkinning::create(_w,_character, _skeleton);

   return true;
}

void LinearBlendSkinning::clear()
{
   SkeletonSkinning::clear();
}

void LinearBlendSkinning::deform()
{
   ulong vertexNumber = character->getNumVertices();
   const std::vector<SkeletonNode> & skelNodes = skeleton->getNodesVector();

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<vertexNumber; ++i)
   {
      cg3::Transform T;

      T.setToZero();
      const std::vector<unsigned long> & bones = w->getNonZeros(i);
      const std::vector<double> & vWeights = w->getWeights(i);

      for(ulong j=0; j<bones.size(); ++j)
      {
         const ulong b = bones[j];
         const double wb = vWeights[b];
         const cg3::Transform & Tb = skelNodes[b].getGlobalT();
         T = T + (Tb*wb);
      }

      cg3::Point3d v = character->getRestPoseVertex(i);
      cg3::Point3d v1 = T.applyToPoint(v);

      character->setVertex(i, v1);
      //character->setSkelPoseVertex(i, v1);
   }

}
