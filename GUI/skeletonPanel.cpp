#include "skeletonPanel.h"
#include "ui_skeletonPanel.h"

#include "controller.h"
#include "drawables/drawableSkeleton.h"
#include "GUI/glCanvas.h"
#include "common/skeletonOperations.h"
#include "skinning/skeletonSkinning.h"

SkeletonPanel::SkeletonPanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::SkeletonPanel)
{
   ui->setupUi(this);
}

SkeletonPanel::~SkeletonPanel()
{
   delete ui;
}

void SkeletonPanel::on_drawSkelCB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonLoaded)
   {
      c->skeleton->activateSkeletonRendering(checked);
      c->glCanvas->update();
   }
}

void SkeletonPanel::on_lbsRB_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized)
   {
      c->skeletonSkinning = (SkeletonSkinning *)c->lbs;

      c->glCanvas->update();
   }
}

void SkeletonPanel::on_dqsRB_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized)
   {
      c->skeletonSkinning = (SkeletonSkinning *)c->dqs;

      c->glCanvas->update();
   }
}

void SkeletonPanel::on_corRB_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized && c->cor!=nullptr)
   {
      c->skeletonSkinning = (SkeletonSkinning *)c->cor;

      c->glCanvas->update();
   }
   else
   {
      ui->lbsRB->setChecked(true);
   }
}
