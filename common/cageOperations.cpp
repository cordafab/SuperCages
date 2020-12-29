#include "cageOperations.h"

#include <vector>

#include "GUI/qtUtils.h"
#include "common/importFiles.h"
#include "common/exportFiles.h"
#include "controller.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"
#include "drawables/drawableCharacter.h"
#include "GUI/glCanvas.h"
#include "skinning/noCageSkinning.h"
#include "skinning/meanValueCoordinates.h"
#include "skinning/cageSkinning.h"
#include "operators/cageUpdater.h"
#include "GUI/restPoseCanvas.h"
#include "operators/cageReverser.h"
#include "skinning/skeletonSkinning.h"

void saveCageWeightsToFile()
{
    std::string filename;
    Controller * c = Controller::get();

    if (openFileSaveDialog(filename, "Save Weights", "Text File (*.txt)"))
    {
        if(c->areCageWeightsLoaded)
        {
            saveWeights(filename.c_str(), c->cageWeights);
        }
    }

    updateGUI();

}


void clearCage()
{
   Controller * c = Controller::get();

   c->isCageLoaded = false;
   c->isCageDeformerActive = false;
   c->glCanvas->removeDrawableObject(c->cage);
   c->glCanvas->removePickableObject(c->cage);
   c->cage->clear();

   c->isCageSkinningInitialized = false;
   c->nc->clear();
   c->mvc->clear();
   //No cageSkinning.Clear

   c->areCageWeightsLoaded = false;
   c->cageWeights->clear();

   c->isCageUpdaterInitialized = false;
   c->isCageUpdaterActive = false;
   c->cageUpdater->clear();

   c->cageReverser->clear();
   c->selectedVerticesForInversion.clear();

   updateGUI();
}

void updateCageInfluenceTexture()
{
    Controller * c = Controller::get();

    if(c->isCageWeightsRenderActive)
    {

        std::vector<float> tex1D(c->character->getNumVertices());

        const std::vector<bool> isVertexSelected = c->cage->getSelectedVertices();

        for(int i=0; i < isVertexSelected.size(); i++)
        {
            if(isVertexSelected[i])
            {
                for(int j=0; j<c->character->getNumVertices();j++){
                    tex1D[j] += c->cageWeights->getWeight(i,j);
                    if (tex1D[j]>0.99) tex1D[j]=0.99;
                    if (tex1D[j]<0.01) tex1D[j]=0.01;
                }
            }
        }
        c->character->updateTexture1DCoords(tex1D);
    }

    updateGUI();

}

void switchCageDeformation()
{
    Controller * c = Controller::get();

    c->isCageDeformerActive = !c->isCageDeformerActive;
    c->cage->activateTransformation = !c->cage->activateTransformation;
    updateGUI();
}

void computeCageWeights()
{
   Controller * c = Controller::get();
   //Compute MeanValueCoordinates
   if(c->isCharacterLoaded && c->isCageLoaded)
   {

      c->nc->create(c->character, c->cage);

      c->mvc->create(c->character,
                     c->cage);

      c->cageWeights = c->mvc->getWeights();
      c->cageSkinning = c->mvc;

      c->areCageWeightsLoaded = true;
      c->isCageSkinningInitialized = true;
      c->isCageDeformerActive = true;
   }
}

void initializeCageUpdater()
{
    Controller * c = Controller::get();
    if(c->isCageSkinningInitialized)
    {
        if( c->selectedVerticesForInversion.size() == 0 )
        {
            // THEN DO COMPUTATIONS BASED ON ALL MESH VERTICES :
            c->cageUpdater->create(c->cageWeights,
                                             c->character,
                                             c->cage);
            c->cageReverser->create(c->character,
                                                   c->cage,
                                                   c->skeleton,
                                                   c->skeletonUpdaterWeights,
                                                   c->cageWeights,
                                                   c->skeletonWeights);
        }
        else {
            // THEN DO COMPUTATIONS BASED ON SELECTED MESH VERTICES ONLY :
            std::cout << "  initializeCageUpdater  :  new CageUpdater" << std::endl;
            c->cageUpdater->create(c->cageWeights,
                                   c->character,
                                   c->cage ,
                                   c->selectedVerticesForInversion);
            std::cout << "  initializeCageUpdater  :  new CageUpdater  IS OK" << std::endl;

            std::cout << "  initializeCageUpdater  :  new CageTranslator" << std::endl;
            c->cageReverser->create(c->character,
                                                   c->cage,
                                                   c->skeleton,
                                                   c->skeletonUpdaterWeights,
                                                   c->cageWeights,
                                                   c->skeletonWeights ,
                                                   c->selectedVerticesForInversion);
            std::cout << "  initializeCageUpdater  :  new CageTranslator  IS OK" << std::endl;
        }

        c->isCageUpdaterInitialized = true;
        c->isCageUpdaterActive = true;
    }
}

void switchCageUpdater()
{
    Controller * c = Controller::get();

    c->isCageUpdaterActive = !c->isCageUpdaterActive;
    updateGUI();
}

void switchSkelUpdater()
{
    Controller * c = Controller::get();

    c->isSkeletonUpdaterActive = !c->isSkeletonUpdaterActive;

    if(!c->isSkeletonUpdaterActive)
    {
       c->skeletonSkinning->rootMotion = false;
       c->skeleton->isRootMotionEnabled = false;
    } else
    {
       c->skeletonSkinning->rootMotion = c->isRootMotionEnabled;
       c->skeleton->isRootMotionEnabled = c->isRootMotionEnabled;
    }

    updateGUI();
}
