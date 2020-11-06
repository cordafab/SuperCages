#ifndef CORSKINNING_H
#define CORSKINNING_H

#include "skinning/skeletonSkinning.h"
#include "skinning/cageSkinning.h"
#include "drawables/drawableSkeleton.h"

class CoRSkinning:
      public SkeletonSkinning
{
public:
   CoRSkinning();
   CoRSkinning(Weights   * _w,
               Character * _character,
               Cage      * _cage,
               Skeleton  * _skeleton,
               Weights   * _corWeights);

   bool create(Weights   * _w,
               Character * _character,
               Cage      * _cage,
               Skeleton  * _skeleton,
               Weights   * _corWeights);

   void clear();

   void deform();
   void updateCoRs();

private:
   Weights                  * corWeights;
   Cage                     * cage;
   std::vector<cg3::Vec3d>    cor;
};

#endif // CORSKINNING_H
