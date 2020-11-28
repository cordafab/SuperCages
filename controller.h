#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>

//Forward declarations
class DrawableCharacter;
class DrawableCage;
class DrawableSkeleton;

class DrawableObject;

class Weights;

class CageSkinning;
class NoCageSkinning;
class MeanValueCoordinates;

class SkeletonSkinning;
class LinearBlendSkinning;
class DualQuaternionSkinning;
class CoRSkinning;

class AsyncAnimator;

class MainWindow;
class GlCanvas;
class RigPanel;
class CharacterPanel;
class CagePanel;
class SkeletonPanel;
class AnimatorPanel;
class AsyncAnimatorPanel;
class ToolsPanel;

class CageReverser;
class CageUpdater;
class SkeletonUpdater;

class RestPoseCanvas;

class Controller
{
public:

   static Controller* get();

   //Data
   DrawableCharacter      * character;
   DrawableCage           * cage;
   DrawableSkeleton       * skeleton;

   //Skinning
   Weights                * cageWeights;
   CageSkinning           * cageSkinning;

   NoCageSkinning         * nc;
   MeanValueCoordinates   * mvc;

   Weights                * skeletonWeights;
   SkeletonSkinning       * skeletonSkinning;

   Weights                * skeletonUpdaterWeights;

   LinearBlendSkinning    * lbs;
   DualQuaternionSkinning * dqs;
   CoRSkinning            * cor;

   Weights                * corWeights;

   //Animation
   AsyncAnimator          * asyncAnimator;

   //SuperCages
   CageReverser           * cageReverser;
   CageUpdater            * cageUpdater;
   SkeletonUpdater        * skeletonUpdater;

   //TO DO: Move this in another class
   std::vector< int >       selectedVerticesForInversion;

   //Flags
   bool                     isCharacterLoaded;
   bool                     isCageLoaded;
   bool                     isSkeletonLoaded;

   bool                     areCageWeightsLoaded;
   bool                     isCageSkinningInitialized;

   bool                     areSkeletonWeightsLoaded;
   bool                     isSkeletonSkinningInitialized;

   bool                     isSkeletonWeightsRenderActive;
   bool                     isCageWeightsRenderActive;
   bool                     isSkeletonUpdaterWeightsRenderActive;

   bool                     isSkeletonDeformerActive;
   bool                     isCageDeformerActive;

   bool                     isSkeletonCageBlendActive;

   bool                     isAnimatorInitialized;
   bool                     isAnimatorActivated;

   bool                     isDiffuseTextureInitialized;

   bool                     isRootMotionEnabled;

   bool                     isCageUpdaterInitialized;
   bool                     isCageUpdaterActive;

   bool                     isSkeletonUpdaterInitialized;
   bool                     isSkeletonUpdaterActive;

   //GUI
   MainWindow             * mainWindow;
   GlCanvas               * glCanvas;
   RigPanel               * rigPanel;
   CharacterPanel         * characterPanel;
   CagePanel              * cagePanel;
   SkeletonPanel          * skeletonPanel;
   AnimatorPanel          * animatorPanel;
   AsyncAnimatorPanel     * asyncAnimatorPanel;
   ToolsPanel             * toolsPanel;

   RestPoseCanvas         * restPoseCanvas;

   ~Controller();

protected:

   Controller();

private:

   static Controller * instance;

};

#endif // CONTROLLER_H
