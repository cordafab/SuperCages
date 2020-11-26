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
   //TO DO: Move in skeleton class
   ulong vertexNumber = character->getNumVertices();
   const std::vector<SkeletonNode> & skelNodes = skeleton->getNodesVector();

   #pragma omp parallel for schedule(static)
   for(ulong i=0; i<vertexNumber; ++i)
   {
      //V1
      cg3::Transform T;

      T.setToZero();
      const std::vector<unsigned long> & handles = w->getNonZeros(i);
      const std::vector<double> & vWeights = w->getWeights(i);

      for(ulong j=0; j<handles.size(); ++j)
      {
         const ulong h = handles[j];
         const double wh = vWeights[h];
         const cg3::Transform & Th = skelNodes[h].getGlobalT();
         T = T + (Th*wh);
      }

      cg3::Point3d v = character->getRestPoseVertex(i);
      cg3::Point3d v1 = T.applyToPoint(v);

      if(rootMotion) v1 = skeleton->getRootMotion().applyToPoint(v1);
      character->setVertex(i, v1);
      //character->setSkelPoseVertex(i, v1);
   }

}
