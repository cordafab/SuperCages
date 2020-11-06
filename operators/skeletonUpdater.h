#ifndef SKELETONUPDATER_H
#define SKELETONUPDATER_H

#include "skinning/weights.h"
#include "rigs/skeleton.h"
#include "rigs/cage.h"
#include "rigs/character.h"
#include "geom/transform.h"

class SkeletonUpdater
{
public:
   SkeletonUpdater();
   SkeletonUpdater(Weights   * _wSkeleton,
                   Weights   * _wCage,
                   Character * _character,
                   Skeleton  * _skeleton,
                   Cage      * _cage);


   bool create    (Weights   * _wSkeleton,
                   Weights   * _wCage,
                   Character * _character,
                   Skeleton  * _skeleton,
                   Cage      * _cage);

   void clear();

   //wrt the cage
   static bool generateSkeletonUpdaterWeights(Weights   * & skeletonUpdaterWeights,
                                              Weights   *   skeletonWeights,
                                              Weights   *   cageWeights,
                                              Character *   character,
                                              Skeleton  *   skeleton,
                                              Cage      *   cage);

   //wrt the char skin
   static bool generateSkeletonUpdaterWeights(Weights   * & skeletonUpdaterWeights,
                                              Weights   *   skeletonWeights,
                                              Character *   character,
                                              Skeleton  *   skeleton);

   void updatePosition();

   inline Weights * getWeights() { return skeletonUpdaterWeights; }

private:

   inline void setSkeletonWeights(Weights * _w) { wSkel = _w; }
   inline void setCageWeights(Weights * _w) { wCage = _w; }
   inline void setCharacter(Character * _character) { character = _character; }
   inline void setCage(Cage * _cage) { cage = _cage; }
   inline void setSkeleton(Skeleton * _skeleton) { skeleton = _skeleton; }

protected:
   Weights   * wSkel;
   Weights   * wCage;
   Character * character;
   Skeleton  * skeleton;
   Cage      * cage;

   Weights   * skeletonUpdaterWeights;

   std::vector<cg3::Vec3d> originalNodePositions;

};

#endif // SKELETONUPDATER_H
