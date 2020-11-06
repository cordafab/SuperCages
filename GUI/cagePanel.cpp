#include "cagePanel.h"
#include "ui_cagePanel.h"

#include "controller.h"
#include "drawables/drawableCage.h"
#include "GUI/glCanvas.h"

#include "common/cageOperations.h"
#include "skinning/cageSkinning.h"

//TEST
#include "drawables/drawableCharacter.h"
#include "Eigen/SparseCore"
#include "skinning/weights.h"
#include "GUI/qtUtils.h"
#include "common/importFiles.h"
#include "common/exportFiles.h"
//#include "cinolib/linear_solvers.h"

CagePanel::CagePanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::CagePanel)
{
   ui->setupUi(this);

   ui->weightsExport->setIcon(QIcon(QPixmap(":/GUI/icons/save.png")));
   ui->weightsExport->setStyleSheet("padding:5px; text-align:left;");
   ui->weightsExport->setIconSize(QSize(18, 18));


}

CagePanel::~CagePanel()
{
   delete ui;
}

void CagePanel::on_drawCageCB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isCageLoaded)
   {
      c->cage->activateCageRendering(checked);
      c->glCanvas->update();
   }
}

void CagePanel::on_weightsExport_clicked()
{
    saveCageWeightsToFile();
}

void CagePanel::on_noCoordinates_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->areCageWeightsLoaded)
   {
      c->cageSkinning = (CageSkinning *)c->nc;
      c->cageWeights = c->cageSkinning->getWeights();
      c->glCanvas->update();
   }
}

void CagePanel::on_mvcCoordinates_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->areCageWeightsLoaded)
   {
      c->cageSkinning = (CageSkinning *)c->mvc;
      c->cageWeights = c->cageSkinning->getWeights();
      c->glCanvas->update();
   }
}
