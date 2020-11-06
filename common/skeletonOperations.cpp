#include "skeletonOperations.h"

#include "controller.h"
#include "GUI/glCanvas.h"
#include "drawables/drawableCharacter.h"
#include "drawables/drawableSkeleton.h"
#include "skinning/skeletonSkinning.h"
#include "GUI/qtUtils.h"
#include "common/exportFiles.h"
#include "operators/skeletonUpdater.h"
#include "drawables/drawableCage.h"
#include "skinning/linearBlendSkinning.h"
#include "skinning/dualQuaternionSkinning.h"
#include "skinning/corSkinning.h"

void clearSkeleton()
{
   Controller * c = Controller::get();

   c->isSkeletonLoaded = false;
   c->isSkeletonDeformerActive = false;
   c->glCanvas->removeDrawableObject(c->skeleton);
   c->glCanvas->removePickableObject(c->skeleton);
   c->skeleton->clear();

   c->areSkeletonWeightsLoaded = false;
   c->skeletonWeights->clear();

   c->isSkeletonSkinningInitialized = false;
   c->lbs->clear();
   c->dqs->clear();
   c->cor->clear();
   //No SkeletonSkinning.clear()

   c->isSkeletonUpdaterInitialized = false;
   c->isSkeletonUpdaterActive = false;
   c->skeletonUpdater->clear();

   updateGUI();
}

void updateSkeletonInfluenceTexture(bool renderOnlyTheFather)
{
   Controller * c = Controller::get();

   if(c->isSkeletonWeightsRenderActive)
   {
      std::vector<float> tex1D(c->character->getNumVertices());

      if(!renderOnlyTheFather)
      {
         //Rendering with "Forward selection"
         const std::vector<bool> isNodeSelected = c->skeleton->getSelectedNodes();
         for(int i=0; i < isNodeSelected.size(); i++)
         {
            if(isNodeSelected[i])
            {
               for(int j=0; j<c->character->getNumVertices();j++){
                  tex1D[j] += c->skeletonWeights->getWeight(i,j);
                  if (tex1D[j]>0.99) tex1D[j]=0.99;
                  if (tex1D[j]<0.01) tex1D[j]=0.01;
               }
            }
         }
      }
      else
      {
         //Rendering for father only weights
         int i = c->skeleton->getSelectedFatherNode();
         if(i!=-1)
         {
            for(int j=0; j<c->character->getNumVertices();j++){
               float w = c->skeletonWeights->getWeight(i,j);
               tex1D[j] += w;
               if (tex1D[j]>0.99){ tex1D[j]=0.99; }
               if (tex1D[j]<0.01){ tex1D[j]=0.01; }
            }
         }
      }

      c->character->updateTexture1DCoords(tex1D);
   }

   updateGUI();

}

void updateSkeletonUpdaterInfluenceTexture()
{
   Controller * c = Controller::get();

   if(c->isSkeletonUpdaterWeightsRenderActive)
   {
      std::vector<float> tex1D(c->character->getNumVertices());

      //Rendering for father only weights
      int i = c->skeleton->getSelectedFatherNode();
      if(i!=-1)
      {
         for(int j=0; j<c->character->getNumVertices();j++)
         {
            float w = 0.0;
            for(int h=0; h<c->cage->getNumVertices(); ++h)
            {
               //the skeleton is influenced by the cage, but the texture refers to the char skin
               tex1D[j] += c->skeletonUpdaterWeights->getWeight(h,i) * c->cageWeights->getWeight(h,j);
            }

         }
         double max = 0.000000001;
         for(int j=0; j<tex1D.size(); ++j)
         {
            if(tex1D[j] > max) max = tex1D[j];
         }
         for(int j=0; j<tex1D.size(); ++j)
         {
            tex1D[j] /= max;
         }
         for(int j=0; j<tex1D.size(); ++j)
         {
            if (tex1D[j]>0.99){ tex1D[j]=0.99; }
            if (tex1D[j]<0.01){ tex1D[j]=0.01; }
         }
      }

      c->character->updateTexture1DCoords(tex1D);
   }

   updateGUI();

}

void switchSkeletonDeformation()
{
   Controller * c = Controller::get();

   c->isSkeletonDeformerActive = !c->isSkeletonDeformerActive;
   c->skeleton->activateTransformation = !c->skeleton->activateTransformation;
   updateGUI();
}

void saveSkelWeightsToFile()
{
   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save Weights", "Text File (*.txt)"))
   {
      if(c->areSkeletonWeightsLoaded)
      {
         saveWeights(filename.c_str(), c->skeletonWeights);
      }
   }

   updateGUI();

}

void initializeSkeletonUpdater()
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized && c->isCageSkinningInitialized)
   {

      //const std::vector<Node> nodes = c->skeleton->getNodesVector();
      /*for(unsigned int j = 0 ; j < nodes.size(); ++j)
      {
         if(nodes[j].isItFake())
         {
            int fatherIndex = nodes[j].getFather();
            for(unsigned int v = 0; v < c->character->getNumVertices(); ++v)
            {
               double fatherWeight = c->skeletonWeights->getWeight(fatherIndex, v);
               c->skeletonWeights->setWeight(j,v,fatherWeight);
            }
         }
      }*/

      c->skeletonUpdater->create(c->skeletonWeights,
                                               c->cageWeights,
                                               c->character,
                                               c->skeleton,
                                               c->cage);
      c->skeletonUpdaterWeights = c->skeletonUpdater->getWeights();

      c->isSkeletonUpdaterInitialized = true;
      c->isSkeletonUpdaterActive = true;
   }

}

void addFakeBones(Skeleton * skeleton) //Is it necessary?
{
   std::vector<Node> nodesCopy = skeleton->getNodesVector();
   for(int i=0; i<nodesCopy.size(); ++i)
   {
      if(nodesCopy[i].getNext().size()==0)
      {
         std::stringstream ss;
         ss << "Fake " << i;
         std::string s = ss.str();
         //skeleton->addNode(s,i,cg3::Transform(),nodesCopy[i].getGlobalRestPose(), true);
      }
   }
}
