#include "toolsOperation.h"
#include "GUI/toolsPanel.h"

#include "controller.h"
#include "GUI/glCanvas.h"
#include "GUI/restPoseCanvas.h"
#include "drawables/drawableCharacter.h"

//TEST
#include "common/exportFiles.h"
#include "GUI/qtUtils.h"


void updateToolsGUI()
{
   Controller * c = Controller::get();

   c->toolsPanel->updateSkelDeformationButton();
   c->toolsPanel->updateCageDeformationButton();
   c->toolsPanel->updateCageSkelUpdaterButton();
}

void saveCamera()
{
   Controller * c = Controller::get();

   c->glCanvas->saveCamera();
}

void restoreCamera()
{
   Controller * c = Controller::get();

   c->glCanvas->restoreCamera();
}
