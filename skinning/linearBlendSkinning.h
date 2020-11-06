#ifndef LINEARBLENSSKINNING_H
#define LINEARBLENSSKINNING_H

#include "skinning/skeletonSkinning.h"
#include "drawables/drawableSkeleton.h"


class LinearBlendSkinning:
      public SkeletonSkinning
{
public:
   LinearBlendSkinning();
   LinearBlendSkinning(Weights * _w, Character *_character, Skeleton *_skeleton);

   bool create(Weights * _w, Character *_character, Skeleton *_skeleton);

   void clear();

   void deform();
};

#endif // LINEARBLENSSKINNING_H
