#include "characterPanel.h"
#include "ui_characterPanel.h"

#include "controller.h"
#include "drawables/drawableCharacter.h"
#include "GUI/glCanvas.h"

#include "common/characterOperations.h"
#include "common/cageOperations.h"
#include "common/skeletonOperations.h"

#include "skinning/skeletonSkinning.h"
#include "skinning/cageSkinning.h"

#include "common/exportFiles.h"
#include "GUI/qtUtils.h"

CharacterPanel::CharacterPanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::CharacterPanel)
{
   ui->setupUi(this);

   ui->exportChar->setIcon(QIcon(QPixmap(":/GUI/icons/save.png")));
   ui->exportChar->setStyleSheet("padding:5px; text-align:left;");
   ui->exportChar->setIconSize(QSize(18, 18));
}

CharacterPanel::~CharacterPanel()
{
   delete ui;
}

void CharacterPanel::on_drawTrimeshCB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded)
   {
      c->character->activateCharacterRendering(checked);
      c->glCanvas->update();
   }
}

void CharacterPanel::on_smoothColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded)
   {
      c->isCageWeightsRenderActive = false;
      c->isSkeletonWeightsRenderActive = false;
      c->character->activateSmoothColouration();
      c->glCanvas->update();

   }
}

void CharacterPanel::on_flatColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded)
   {
      c->isCageWeightsRenderActive = false;
      c->isSkeletonWeightsRenderActive = false;
      c->character->activateFlatColouration();
      c->glCanvas->update();
   }
}

void CharacterPanel::on_wireColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded)
   {
      c->isCageWeightsRenderActive = false;
      c->isSkeletonWeightsRenderActive = false;
      c->character->activateWireframe();
      c->glCanvas->update();
   }
}

void CharacterPanel::on_cageColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded          &&
      c->isCageLoaded               &&
      c->isCageSkinningInitialized  &&
      c->areCageWeightsLoaded         )
   {
      c->isCageWeightsRenderActive = true;
      c->isSkeletonWeightsRenderActive = false;
      c->isSkeletonUpdaterWeightsRenderActive = false;
      updateCageInfluenceTexture();
      c->character->activateTexture1D();
      c->glCanvas->update();
   }
}

void CharacterPanel::on_skelColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded              &&
      c->isSkeletonLoaded               &&
      c->isSkeletonSkinningInitialized  &&
      c->areSkeletonWeightsLoaded         )
   {
      c->isSkeletonWeightsRenderActive = true;
      c->isCageWeightsRenderActive = false;
      c->isSkeletonUpdaterWeightsRenderActive = false;
      updateSkeletonInfluenceTexture(false);
      c->character->activateTexture1D();
      c->glCanvas->update();
   }
}

void CharacterPanel::on_skelUpdtrColourRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded              &&
      c->isSkeletonLoaded               &&
      c->isSkeletonUpdaterInitialized    )
   {
      c->isSkeletonUpdaterWeightsRenderActive = true;
      c->isCageWeightsRenderActive = false;
      c->isSkeletonWeightsRenderActive = false;
      updateSkeletonUpdaterInfluenceTexture();
      c->character->activateTexture1D();
      c->glCanvas->update();
   }
}

void CharacterPanel::on_texturedRB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded              &&
      c->isSkeletonLoaded               &&
      c->isSkeletonSkinningInitialized  &&
      c->areSkeletonWeightsLoaded       &&
      c->isDiffuseTextureInitialized     )
   {
      c->isSkeletonWeightsRenderActive = false;
      c->isCageWeightsRenderActive = false;
      c->isSkeletonUpdaterWeightsRenderActive = false;
      c->character->updateTexture2D(1.0, c->character->diffuse_tex.c_str());
      c->character->activateTexture2D();
      c->glCanvas->update();
   }

}

void CharacterPanel::on_skelSkin_clicked()
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded              &&
      c->isSkeletonSkinningInitialized   )
   {
      c->skeletonSkinning->deform();
      c->character->updateNormals();
      c->character->updateCutVerticesPosition();
      c->glCanvas->updateGL();
   }
}

void CharacterPanel::on_cageSkin_clicked()
{
   Controller * c = Controller::get();
   if(c->isCharacterLoaded              &&
      c->isCageSkinningInitialized   )
   {
      c->cageSkinning->deform();
      c->character->updateNormals();
      c->character->updateCutVerticesPosition();
      c->glCanvas->updateGL();
   }
}

void CharacterPanel::on_exportChar_clicked()
{
   Controller * controller = Controller::get();
   std::string filepath;
   if (openFileSaveDialog(filepath, "Save Character", "3D Mesh (*.obj)"))
   {
      std::vector<double> v = controller->character->getVerticesVector();
      std::vector<int>    f = controller->character->getTrianglesVector();

      std::string finalName = (filepath+".obj");
      saveOBJ(finalName.c_str(), v, f);
   }
}
