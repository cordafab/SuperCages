#ifndef DUALQUATERNIONSKINNING_H
#define DUALQUATERNIONSKINNING_H

#include "skinning/skeletonSkinning.h"
#include "drawables/drawableSkeleton.h"


class DualQuaternionSkinning:
      public SkeletonSkinning
{
public:
   DualQuaternionSkinning();
   DualQuaternionSkinning(Weights * _w, Character *_character, Skeleton *_skeleton);

   bool create(Weights * _w, Character *_character, Skeleton *_skeleton);

   void clear();

   void deform();
};


#endif // DUALQUATERNIONSKINNING_H
