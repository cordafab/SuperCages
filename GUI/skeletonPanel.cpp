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
      bool isRootMotionEnabled = c->skeletonSkinning->rootMotion;
      c->isRootMotionEnabled = isRootMotionEnabled;
      c->skeleton->isRootMotionEnabled = isRootMotionEnabled;
      if(isRootMotionEnabled) ui->rootMotionCB->setCheckState(Qt::CheckState::Checked);
      else                    ui->rootMotionCB->setCheckState(Qt::CheckState::Unchecked);
      c->glCanvas->update();
   }
}

void SkeletonPanel::on_dqsRB_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized)
   {
      c->skeletonSkinning = (SkeletonSkinning *)c->dqs;
      bool isRootMotionEnabled = c->skeletonSkinning->rootMotion;
      c->isRootMotionEnabled = isRootMotionEnabled;
      c->skeleton->isRootMotionEnabled = isRootMotionEnabled;
      if(isRootMotionEnabled) ui->rootMotionCB->setCheckState(Qt::CheckState::Checked);
      else                    ui->rootMotionCB->setCheckState(Qt::CheckState::Unchecked);
      c->glCanvas->update();
   }
}

void SkeletonPanel::on_rootMotionCB_clicked(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonUpdaterInitialized)
   {
      c->isRootMotionEnabled = checked;
      c->skeletonSkinning->rootMotion = checked;
      c->skeleton->isRootMotionEnabled = checked;
   }
}

void SkeletonPanel::on_corRB_toggled(bool checked)
{
   Controller * c = Controller::get();
   if(c->isSkeletonSkinningInitialized && c->cor!=nullptr)
   {
      c->skeletonSkinning = (SkeletonSkinning *)c->cor;
      bool isRootMotionEnabled = c->skeletonSkinning->rootMotion;
      c->isRootMotionEnabled = isRootMotionEnabled;
      c->skeleton->isRootMotionEnabled = isRootMotionEnabled;
      if(isRootMotionEnabled) ui->rootMotionCB->setCheckState(Qt::CheckState::Checked);
      else                    ui->rootMotionCB->setCheckState(Qt::CheckState::Unchecked);
      c->glCanvas->update();
   }
   else
   {
      ui->lbsRB->setChecked(true);
   }
}
