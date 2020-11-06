#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "controller.h"

#include "drawables/drawableCharacter.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"

#include "skinning/weights.h"

#include "skinning/noCageSkinning.h"
#include "skinning/meanValueCoordinates.h"

#include "skinning/linearBlendSkinning.h"
#include "skinning/dualQuaternionSkinning.h"
#include "skinning/corSkinning.h"

#include "animation/asyncAnimator.h"

#include "operators/cageReverser.h"
#include "operators/cageUpdater.h"
#include "operators/skeletonUpdater.h"

class Environment
{
public:
   Environment();
   ~Environment();

private:
   void initController();

// Data
   DrawableCharacter            character;
   DrawableCage                 cage;
   DrawableSkeleton             skeleton;

// Skinning

// CageSkinning                 cageSkinning;
   Weights                      cageWeights;

   NoCageSkinning               nc;
   MeanValueCoordinates         mvc;

// SkeletonSkinning             skeletonSkinning;
   Weights                      skeletonWeights;

   LinearBlendSkinning          lbs;
   DualQuaternionSkinning       dqs;
   CoRSkinning                  cor;

   Weights                      corWeights;

// Animation
   AsyncAnimator                asyncAnimator;

// SuperCages
   CageReverser               cageTranslator;
   CageUpdater                  cageUpdater;
   SkeletonUpdater              skeletonUpdater;

   Weights                      skeletonUpdaterWeights;
};

#endif // ENVIRONMENT_H
