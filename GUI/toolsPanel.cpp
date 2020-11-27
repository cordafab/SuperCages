#include "toolsPanel.h"
#include "ui_toolsPanel.h"

#include "controller.h"
#include "common/skeletonOperations.h"
#include "common/cageOperations.h"
#include "operators/cageUpdater.h"
#include "common/toolsOperation.h"
#import "common/importFiles.h"
#import  "common/exportFiles.h"

#include "GUI/qtUtils.h"
#include "GUI/glCanvas.h"

//DEBUG
#include "drawables/drawableSkeleton.h"
#include "Eigen/Eigen"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtc/type_ptr.hpp"

ToolsPanel::ToolsPanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::ToolsPanel)
{
   ui->setupUi(this);

   ui->saveCamera->setIcon(QIcon(QPixmap(":/GUI/icons/saveCamera.png")));
   ui->saveCamera->setIconSize(QSize(18, 18));
   ui->saveCamera->setStyleSheet("padding:5px; text-align:left;");

   ui->restoreCamera->setIcon(QIcon(QPixmap(":/GUI/icons/camera.png")));
   ui->restoreCamera->setIconSize(QSize(18, 18));
   ui->restoreCamera->setStyleSheet("padding:5px; text-align:left;");

   ui->exportCamera->setIcon(QIcon(QPixmap(":/GUI/icons/saveCamera.png")));
   ui->exportCamera->setIconSize(QSize(18, 18));
   ui->exportCamera->setStyleSheet("padding:5px; text-align:left;");

   ui->importCamera->setIcon(QIcon(QPixmap(":/GUI/icons/camera.png")));
   ui->importCamera->setIconSize(QSize(18, 18));
   ui->importCamera->setStyleSheet("padding:5px; text-align:left;");
}

ToolsPanel::~ToolsPanel()
{
   delete ui;
}

void ToolsPanel::updateSkelDeformationButton()
{
   Controller * c = Controller::get();

   if(c->isSkeletonDeformerActive)
   {
      ui->skelDeformation->setIcon(QIcon(QPixmap(":/GUI/icons/unlock.png")));
      ui->skelDeformation->setIconSize(QSize(18, 18));
      ui->skelDeformation->setStyleSheet("padding:5px; text-align:left;");
      ui->skelDeformation->setText("Skel");
   }
   else
   {
      ui->skelDeformation->setIcon(QIcon(QPixmap(":/GUI/icons/lock.png")));
      ui->skelDeformation->setIconSize(QSize(18, 18));
      ui->skelDeformation->setStyleSheet("padding:5px; text-align:left;");
      ui->skelDeformation->setText("Skel");
   }

   /*if(c->isSkeletonDeformerActive)
   {
      ui->skelDeformation->setText("SkelON");
   }
   else
   {
      ui->skelDeformation->setText("SkelOFF");
   }*/
}

void ToolsPanel::updateCageDeformationButton()
{
   Controller * c = Controller::get();

   if(c->isCageDeformerActive)
   {
      ui->cageDeformation->setIcon(QIcon(QPixmap(":/GUI/icons/unlock.png")));
      ui->cageDeformation->setIconSize(QSize(18, 18));
      ui->cageDeformation->setStyleSheet("padding:5px; text-align:left;");
      ui->cageDeformation->setText("Cage");
   }
   else
   {
      ui->cageDeformation->setIcon(QIcon(QPixmap(":/GUI/icons/lock.png")));
      ui->cageDeformation->setIconSize(QSize(18, 18));
      ui->cageDeformation->setStyleSheet("padding:5px; text-align:left;");
      ui->cageDeformation->setText("Cage");
   }
}

void ToolsPanel::updateCageSkelUpdaterButton()
{
   Controller * c = Controller::get();

   if(c->isCageUpdaterActive)
   {
      ui->cageUpdater->setIcon(QIcon(QPixmap(":/GUI/icons/on.png")));
      ui->cageUpdater->setIconSize(QSize(9, 9));
      ui->cageUpdater->setStyleSheet("padding:5px; text-align:left;");
      ui->cageUpdater->setText("CgUpdater");
   }
   else
   {
      ui->cageUpdater->setIcon(QIcon(QPixmap(":/GUI/icons/off.png")));
      ui->cageUpdater->setIconSize(QSize(9, 9));
      ui->cageUpdater->setStyleSheet("padding:5px; text-align:left;");
      ui->cageUpdater->setText("CgUpdater");
   }

   if(c->isSkeletonUpdaterActive)
   {
      ui->skelUpdater->setIcon(QIcon(QPixmap(":/GUI/icons/on.png")));
      ui->skelUpdater->setIconSize(QSize(9, 9));
      ui->skelUpdater->setStyleSheet("padding:5px; text-align:left;");
      ui->skelUpdater->setText("SkUpdater");
   }
   else
   {
      ui->skelUpdater->setIcon(QIcon(QPixmap(":/GUI/icons/off.png")));
      ui->skelUpdater->setIconSize(QSize(9, 9));
      ui->skelUpdater->setStyleSheet("padding:5px; text-align:left;");
      ui->skelUpdater->setText("SkUpdater");
   }
}

void ToolsPanel::on_skelDeformation_clicked()
{
   switchSkeletonDeformation();
}

void ToolsPanel::on_cageDeformation_clicked()
{
   switchCageDeformation();
}


void ToolsPanel::on_saveCamera_clicked()
{
   saveCamera();
}

void ToolsPanel::on_restoreCamera_clicked()
{
   restoreCamera();
}

void ToolsPanel::on_cageUpdater_clicked()
{
   switchCageUpdater();
}

void ToolsPanel::on_skelUpdater_clicked()
{
   switchSkelUpdater();
}

void ToolsPanel::on_exportCamera_clicked()
{

   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save Camera", "Camera File (*.txt)"))
   {
      saveCamera();
      std::vector<double> params;
      c->glCanvas->getCameraParams(params);
      saveDoubleVec(filename.c_str(),params);
   }


}

void ToolsPanel::on_importCamera_clicked()
{

   Controller * c = Controller::get();
   std::string filename;

   if (openFileSelectionDialog(filename, "Load Camera", "Camera File (*.txt)",(QWidget*)c->mainWindow))
   {
      std::vector<double> params;
      loadDoubleVec(filename.c_str(),params);
      c->glCanvas->setCameraParams(params);
      restoreCamera();
   }

}
