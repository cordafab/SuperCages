#include "controller.h"

Controller::Controller()
{
   character                              = nullptr;
   cage                                   = nullptr;
   skeleton                               = nullptr;

   isCharacterLoaded                      = false;
   isCageLoaded                           = false;
   isSkeletonLoaded                       = false;

   cageWeights                            = nullptr;
   cageSkinning                           = nullptr;

   nc                                     = nullptr;
   mvc                                    = nullptr;

   skeletonWeights                        = nullptr;
   skeletonSkinning                       = nullptr;

   lbs                                    = nullptr;
   dqs                                    = nullptr;
   cor                                    = nullptr;

   corWeights                             = nullptr;

   asyncAnimator                          = nullptr;

   cageReverser                           = nullptr;
   cageUpdater                            = nullptr;
   skeletonUpdater                        = nullptr;

   skeletonUpdaterWeights                 = nullptr;

   areSkeletonWeightsLoaded               = false;
   isSkeletonSkinningInitialized          = false;

   areCageWeightsLoaded                   = false;
   isCageSkinningInitialized              = false;

   isCageUpdaterInitialized               = false;
   isCageUpdaterActive                    = false;

   isSkeletonUpdaterInitialized           = false;
   isSkeletonUpdaterActive                = false;

   isSkeletonWeightsRenderActive          = false;
   isCageWeightsRenderActive              = false;
   isSkeletonUpdaterWeightsRenderActive   = false;

   isSkeletonDeformerActive               = false;
   isCageDeformerActive                   = false;

   isSkeletonCageBlendActive              = false;

   isAnimatorInitialized                  = false;
   isAnimatorActivated                    = false;

   isDiffuseTextureInitialized            = false;

   isRootMotionEnabled                    = false;

   mainWindow                             = nullptr;
   glCanvas                               = nullptr;
   rigPanel                               = nullptr;
   characterPanel                         = nullptr;
   cagePanel                              = nullptr;
   skeletonPanel                          = nullptr;
   animatorPanel                          = nullptr;
   asyncAnimatorPanel                     = nullptr;
   toolsPanel                             = nullptr;

   restPoseCanvas                         = nullptr;

   selectedVerticesForInversion.clear();
}

Controller::~Controller()
{

}

Controller* Controller::instance = nullptr;

Controller* Controller::get()
{
    if (instance == nullptr)
        instance = new Controller;
    return instance;
}
