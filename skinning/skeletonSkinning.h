#ifndef SKELETONSKINNING_H
#define SKELETONSKINNING_H

#include "skinning/weights.h"
#include "rigs/character.h"
#include "rigs/skeleton.h"
#include "geom/transform.h"

class SkeletonSkinning
{
public:
   SkeletonSkinning();
   SkeletonSkinning(Weights   * _w,
                    Character * _character,
                    Skeleton  * _skeleton);

   bool create(Weights   * _w,
               Character * _character,
               Skeleton  * _skeleton);

   void clear();

   inline void setWeights(Weights * _w) { w = _w; }
   inline void setCharacter(Character * _character) { character = _character; }
   inline void setSkeleton(Skeleton * _skeleton) { skeleton = _skeleton; }

   virtual void deform() = 0;

protected:
   Weights   * w;
   Character * character;
   Skeleton  * skeleton;
};

#endif // SKELETONSKINNING_H
