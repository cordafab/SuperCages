#include "animatorOperations.h"

#include "GUI/qtUtils.h"
#include "GUI/asyncAnimatorPanel.h"
#include "animation/asyncAnimator.h"
#include "common/importFiles.h"
#include "common/exportFiles.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"
#include "drawables/drawableCharacter.h"
#include "skinning/cageSkinning.h"
#include "skinning/skeletonSkinning.h"
#include "operators/cageUpdater.h"
#include "operators/skeletonUpdater.h"
#include "GUI/glCanvas.h"
#include "GUI/mainWindow.h"
#include "controller.h"
#include <QInputDialog>
#include <vector>

#include <stack>

void initializeAnimator()
{
   Controller * c = Controller::get();

   if(c->isCageLoaded || c->isSkeletonLoaded)
   {
      c->asyncAnimator->create(c->cage, c->skeleton);
      c->isAnimatorInitialized = true;
   }
}

void clearAnimator()
{
   Controller * c = Controller::get();

   c->isAnimatorActivated = false;
   c->isAnimatorInitialized = false;
   c->asyncAnimator->clear();
   c->asyncAnimatorPanel->updateKeyframeList();

   updateGUI();

}

void loadSkelAnimationFromFile()
{

   std::string filename;
   Controller * c = Controller::get();

   if (openFileSelectionDialog(filename, "Load Skel Animation", "Skeleton Animation (*.ska *.txt)"))
   {
      if(c->isAnimatorInitialized)
      {

         std::cout << "Loading animation: " << filename << std::endl;

         double charRadius = 0.0;   c->character->sceneRadius(charRadius);


         std::vector<double> t;
         std::vector<std::vector<cg3::Transform>> skelKeyframes;
         int animationFileVersionNumber;

         loadSkelAnimation(
                  filename.c_str(),
                  t,
                  skelKeyframes,
                  animationFileVersionNumber);

         convertSkelKeyframes(skelKeyframes, c->skeleton, animationFileVersionNumber);

         c->asyncAnimator->loadSkelAnimation(t,skelKeyframes);
      }
   }
}

void convertSkelKeyframes(std::vector<std::vector<cg3::Transform>> & skelKeyframes, Skeleton * skel, uint version)
{

   std::vector<Node> nodes = skel->getNodesVector();

   std::vector<double> m(16);

   if(version==2)
   {
      //Compute the local transform for the current pose keyframes
      for(int i=0; i<skelKeyframes.size(); i++)
      {

         for(int j=0; j<skelKeyframes[i].size(); j++){

            if(nodes[j].getFather()==-1)
            {
              // skelKeyframes[i][j].setTranslation(skelKeyframes[i][j].getTranslation() - nodes[j].getLocalTRest().getTranslation());
            }
            skelKeyframes[i][j] = nodes[j].getLocalTRest().cumulateWith(skelKeyframes[i][j]);

         }
      }
   }

   if(version==3)
   {
      //compute the local transform from the global current pose keyframes
      for(int i=0; i<skelKeyframes.size(); i++)
      {

         std::vector<cg3::Transform> globalTransforms(skelKeyframes[i].size());
         for(int n=0; n<skelKeyframes[i].size(); n++)
         {
            globalTransforms[n] = skelKeyframes[i][n];
         }

         for(int n=0; n<skelKeyframes[i].size(); n++)
         {
            Node & node = nodes[n];
            const int fatherIndex = node.getFather();

            if(fatherIndex != -1)
            {
               cg3::Transform fatherTransformation = globalTransforms[fatherIndex];
               skelKeyframes[i][n] = fatherTransformation.inverse().cumulateWith(globalTransforms[n]);
            }
         }
      }
   }
}

void saveSkelAnimationToFile()
{
   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save Skel Animation", "Skeleton Animation (*.ska *.txt)"))
   {
      if(c->isAnimatorInitialized)
      {
         saveSkelAnimation(filename.c_str(),
                           c->asyncAnimator->getSkelKeyframeTimeVector(),
                           c->asyncAnimator->getSkelKeyframeVector());
      }
   }
}

void loadCageAnimationFromFile()
{

   std::string filename;
   Controller * c = Controller::get();

   if (openFileSelectionDialog(filename, "Load Cage Animation", "Text File (*.txt)"))
   {
      if(c->isAnimatorInitialized)
      {

         std::cout << "Loading animation: " << filename << std::endl;

         std::vector<double> t;
         std::vector<std::vector<double>> cageKeyframes;

         loadCageAnimation(
                  filename.c_str(),
                  t,
                  cageKeyframes);

         c->asyncAnimator->loadCageAnimation(t,cageKeyframes);
      }
   }
}

void saveCageAnimationToFile()
{

   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save CageAnimation", "Text File (*.txt)"))
   {
      if(c->isAnimatorInitialized)
      {
         saveCageAnimation(filename.c_str(),
                           c->asyncAnimator->getCageKeyframeTimeVector(),
                           c->asyncAnimator->getCageKeyframeVector());
      }
   }
}

void quickLoadSkelAnimation(const char * filename)
{
   /*Controller * c = Controller::get();

   if(c->isAnimatorInitialized)
   {
      std::vector<double> t;
      std::vector<std::vector<cg3::Transform>> skelKeyframes;

      loadSkelAnimation(filename,
                        t,
                        skelKeyframes);

      c->asyncAnimator->loadSkelAnimation(t,skelKeyframes);
   }*/
}

void quickLoadCageAnimation(const char * filename)
{
   Controller * c = Controller::get();

   if(c->isAnimatorInitialized)
   {
      std::vector<double> t;
      std::vector<std::vector<double>> cageKeyframes;

      loadCageAnimation(filename,
                        t,
                        cageKeyframes);
      c->asyncAnimator->loadCageAnimation(t,cageKeyframes);
   }
}

void addSkelKeyframe()
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized)
   {

      bool ok;
      double t = QInputDialog::getDouble(c->mainWindow, "Keyframe time (in seconds)",
                                         "Second: ", 0, 0, 2147483647, 6, &ok);
      if (ok && t>=0)
      {
         //skel keyframe
         std::vector<cg3::Transform> skelKeyframes(c->skeleton->getNumNodes());

         for(unsigned long i=0; i<c->skeleton->getNumNodes(); ++i)
         {

            const Node & n = c->skeleton->getNode(i);
            cg3::Transform Tdeformed(n.getLocalTCurrent().getRotation());

            skelKeyframes[i] = Tdeformed;//.cumulateWith(Trest.inverse());
         }

         c->asyncAnimator->addSkelKeyframe(t, skelKeyframes);
      }
   }
}

void addCageKeyframe()
{
   Controller * c = Controller::get();
   if(c->isCageSkinningInitialized)
   {

      bool ok;
      double t = QInputDialog::getDouble(c->mainWindow, "Keyframe time (in seconds)",
                                         "Second: ", 0, 0, 2147483647, 6, &ok);
      if (ok && t>=0)
      {

         //cage keyframe
         const std::vector<double> & baseCage = c->cage->getBasePoseVerticesVector();
         const std::vector<double> & restCage = c->cage->getRestPoseVerticesVector();
         std::vector<double> cageOffsets(restCage.size());
         for(int i=0; i<restCage.size(); ++i)
         {
            cageOffsets[i] = restCage[i] - baseCage[i];
         }

         c->asyncAnimator->addCageKeyframe(t, cageOffsets);
      }
   }
}

void setSkelKeyframe(unsigned long frameIndex)
{
   Controller * c = Controller::get();

   c->asyncAnimator->setSkeletonKeyframeIndex(frameIndex);

   //Skeleton Skinning
   if(c->isSkeletonSkinningInitialized)
   {

      c->skeletonSkinning->deform();

      if(c->isCageUpdaterActive)
      {
         c->cageUpdater->updatePosition();
      }

   }
   c->character->updateNormals();

   c->glCanvas->refreshScene();
}

void setCageKeyframe(unsigned long frameIndex)
{
   Controller * c = Controller::get();

   c->asyncAnimator->setCageKeyframeIndex(frameIndex);

   //Cage Skinning
   if(c->isCageSkinningInitialized)
   {
      c->cageSkinning->deform();

      if(c->isSkeletonUpdaterActive)
      {
         c->skeletonUpdater->updatePosition();
      }

      c->skeletonSkinning->deform();
      c->cageUpdater->updatePosition();
   }

   c->character->updateNormals();

   c->glCanvas->refreshScene();
}

void setNextSkelKeyframe()
{
   Controller * c = Controller::get();

   //Skeleton Skinning
   if((c->isSkeletonSkinningInitialized) && (c->isAnimatorInitialized))
   {
      int frameIndex = c->asyncAnimator->getSkelKeyframeIndex();

      c->asyncAnimator->setSkeletonKeyframeIndex(frameIndex+1);

      c->glCanvas->runSkinningPipeline();
   }
}

void setNextCageKeyframe()
{
   Controller * c = Controller::get();

   if((c->isCageSkinningInitialized) && (c->isAnimatorInitialized))
   {
      int frameIndex = c->asyncAnimator->getCageKeyframeIndex();
      c->asyncAnimator->setCageKeyframeIndex(frameIndex+1);

      c->glCanvas->runSkinningPipeline();
   }
}

void deleteSkelKeyframe(int index)
{
   Controller * c = Controller::get();

   if(c->isAnimatorInitialized)
   {
      c->asyncAnimator->deleteSkelKeyframe(index);

      /*if(c->isCageSkinningInitialized)
      {
         c->asyncAnimator->set;
         c->cageSkinning->deform();
         c->cage->updateNormals();
      }

      c->glCanvas->refreshScene();*/
   }
}

void deleteCageKeyframe(int index)
{
   Controller * c = Controller::get();

   if(c->isAnimatorInitialized)
   {
      c->asyncAnimator->deleteCageKeyframe(index);
   }
}

void editSkelKeyframeTime(int index)
{
   Controller * c = Controller::get();
   if(c->isAnimatorInitialized)
   {

      bool ok;
      double actualTime = c->asyncAnimator->getSkelKeyframeTimeVector()[index];
      double t = QInputDialog::getDouble(c->mainWindow, "Keyframe time (in seconds)",
                                         "Second: ", actualTime, 0, 2147483647, 6, &ok);
      if (ok && t>0)
      {
         c->asyncAnimator->editSkelKeyframeTime(index, t);
      }
   }
}

void editCageKeyframeTime(int index)
{
   Controller * c = Controller::get();
   if(c->isAnimatorInitialized)
   {

      bool ok;
      double actualTime = c->asyncAnimator->getCageKeyframeTimeVector()[index];
      double t = QInputDialog::getDouble(c->mainWindow, "Keyframe time (in seconds)",
                                         "Second: ", actualTime, 0, 2147483647, 6, &ok);
      if (ok && t>0)
      {
         c->asyncAnimator->editCageKeyframeTime(index, t);
      }
   }
}
