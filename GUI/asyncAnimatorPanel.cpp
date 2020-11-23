#include "asyncAnimatorPanel.h"
#include "ui_asyncAnimatorPanel.h"

#include "common/animatorOperations.h"
#include "animation/asyncAnimator.h"
#include "controller.h"
#include "operators/cageReverser.h"
#include "GUI/glCanvas.h"
#include "GUI/restPoseCanvas.h"
#include "GUI/qtUtils.h"
#include "common/importFiles.h"
#include "common/exportFiles.h"
#include "drawables/drawableCharacter.h"
#include "cinolib/string_utilities.h"
#include <QString>

AsyncAnimatorPanel::AsyncAnimatorPanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::AsyncAnimatorPanel)
{
   ui->setupUi(this);

   ui->loadSkelAnimation->setIcon(QIcon(QPixmap(":/GUI/icons/open.png")));
   ui->loadSkelAnimation->setStyleSheet("padding:5px; text-align:left;");
   ui->loadSkelAnimation->setIconSize(QSize(18, 18));

   ui->saveSkelAnimation->setIcon(QIcon(QPixmap(":/GUI/icons/save.png")));
   ui->saveSkelAnimation->setStyleSheet("padding:5px; text-align:left;");
   ui->saveSkelAnimation->setIconSize(QSize(18, 18));

   ui->addSkelKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/add.png")));
   ui->addSkelKeyframe->setIconSize(QSize(18, 18));

   ui->deleteSkelKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/clear.png")));
   ui->deleteSkelKeyframe->setIconSize(QSize(18, 18));

   ui->editSkelTime->setIcon(QIcon(QPixmap(":/GUI/icons/edit.png")));
   ui->editSkelTime->setIconSize(QSize(18, 18));

   ui->nextSkelKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/next.png")));
   ui->nextSkelKeyframe->setIconSize(QSize(18, 18));

   ui->loadCageAnimation->setIcon(QIcon(QPixmap(":/GUI/icons/open.png")));
   ui->loadCageAnimation->setStyleSheet("padding:5px; text-align:left;");
   ui->loadCageAnimation->setIconSize(QSize(18, 18));

   ui->saveCageAnimation->setIcon(QIcon(QPixmap(":/GUI/icons/save.png")));
   ui->saveCageAnimation->setStyleSheet("padding:5px; text-align:left;");
   ui->saveCageAnimation->setIconSize(QSize(18, 18));

   ui->addCageKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/add.png")));
   ui->addCageKeyframe->setIconSize(QSize(18, 18));

   ui->deleteCageKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/clear.png")));
   ui->deleteCageKeyframe->setIconSize(QSize(18, 18));

   ui->editCageTime->setIcon(QIcon(QPixmap(":/GUI/icons/edit.png")));
   ui->editCageTime->setIconSize(QSize(18, 18));

   ui->nextCageKeyframe->setIcon(QIcon(QPixmap(":/GUI/icons/next.png")));
   ui->nextCageKeyframe->setIconSize(QSize(18, 18));

   ui->bakePic->setEnabled(false);
   ui->bakePicRest->setEnabled(false);

#ifdef CUSTOMSNAPSHOTQGL
   ui->bakePic->setEnabled(true);
   ui->bakePicRest->setEnabled(true);
#endif
}

AsyncAnimatorPanel::~AsyncAnimatorPanel()
{
   delete ui;
}

void AsyncAnimatorPanel::updateKeyframeList()
{
   Controller * c = Controller::get();
   ui->skelKeyframeList->clear();
   ui->cageKeyframeList->clear();
   if(c->asyncAnimator!=nullptr)
   {
      const std::vector<double> & tSkel = c->asyncAnimator->getSkelKeyframeTimeVector();
      for(int i=0; i<tSkel.size(); ++i)
      {
         std::string label = std::to_string(i) + " " + std::to_string(tSkel[i]);
         ui->skelKeyframeList->addItem( label.c_str() );
      }

      const std::vector<double> & tCage = c->asyncAnimator->getCageKeyframeTimeVector();
      for(int i=0; i<tCage.size(); ++i)
      {
         std::string label = std::to_string(i) + " " + std::to_string(tCage[i]);
         ui->cageKeyframeList->addItem( label.c_str() );
      }
   }
}

void AsyncAnimatorPanel::on_addSkelKeyframe_clicked()
{
   addSkelKeyframe();
   updateKeyframeList();
}

void AsyncAnimatorPanel::on_addCageKeyframe_clicked()
{
   addCageKeyframe();
   updateKeyframeList();
}

void AsyncAnimatorPanel::on_editSkelTime_clicked()
{
   if(ui->skelKeyframeList->count()>0)
   {
      QList<QListWidgetItem *> items = ui->skelKeyframeList->selectedItems();

      if(items.size()>0)
      {
         QListWidgetItem * item = items[0];
         std::string label = item->text().toStdString();
         std::istringstream iss(label);

         int keyFrameIndex;
         iss >> keyFrameIndex;
         editSkelKeyframeTime(keyFrameIndex);
         updateKeyframeList();
      }
   }
}

void AsyncAnimatorPanel::on_editCageTime_clicked()
{
   if(ui->cageKeyframeList->count()>0)
   {
      QList<QListWidgetItem *> items = ui->cageKeyframeList->selectedItems();

      if(items.size()>0)
      {
         QListWidgetItem * item = items[0];
         std::string label = item->text().toStdString();
         std::istringstream iss(label);

         int keyFrameIndex;
         iss >> keyFrameIndex;
         editCageKeyframeTime(keyFrameIndex);
         updateKeyframeList();
      }
   }
}

void AsyncAnimatorPanel::on_deleteSkelKeyframe_clicked()
{
   if(ui->skelKeyframeList->count()>0)
   {
      QList<QListWidgetItem *> items = ui->skelKeyframeList->selectedItems();

      if(items.size()>0)
      {
         QListWidgetItem * item = items[0];
         std::string label = item->text().toStdString();
         std::istringstream iss(label);

         int keyFrameIndex;
         iss >> keyFrameIndex;

         deleteSkelKeyframe(keyFrameIndex);
         updateKeyframeList();
      }
   }
}

void AsyncAnimatorPanel::on_deleteCageKeyframe_clicked()
{
   if(ui->cageKeyframeList->count()>0)
   {
      QList<QListWidgetItem *> items = ui->cageKeyframeList->selectedItems();

      if(items.size()>0)
      {
         QListWidgetItem * item = items[0];
         std::string label = item->text().toStdString();
         std::istringstream iss(label);

         int keyFrameIndex;
         iss >> keyFrameIndex;
         deleteCageKeyframe(keyFrameIndex);
         updateKeyframeList();
      }
   }
}

void AsyncAnimatorPanel::on_nextSkelKeyframe_clicked()
{
   setNextSkelKeyframe();
   Controller * c = Controller::get();
   c->cageTranslator->skeletonEdited();
}

void AsyncAnimatorPanel::on_nextCageKeyframe_clicked()
{
   setNextCageKeyframe();
}

void AsyncAnimatorPanel::on_loadSkelAnimation_clicked()
{
   loadSkelAnimationFromFile();
   updateKeyframeList();
}

void AsyncAnimatorPanel::on_loadCageAnimation_clicked()
{
   loadCageAnimationFromFile();
   updateKeyframeList();
}

void AsyncAnimatorPanel::on_saveSkelAnimation_clicked()
{
   saveSkelAnimationToFile();
}

void AsyncAnimatorPanel::on_saveCageAnimation_clicked()
{
   saveCageAnimationToFile();
}

void AsyncAnimatorPanel::on_skelKeyframeList_itemClicked(QListWidgetItem *item)
{
   std::string label = item->text().toStdString();
   std::istringstream iss(label);

   int keyFrameIndex;
   iss >> keyFrameIndex;
   setSkelKeyframe(keyFrameIndex);
   Controller * c = Controller::get();
   c->glCanvas->runSkinningPipeline();
   c->cageTranslator->skeletonEdited();
   c->glCanvas->updateGL();
}

void AsyncAnimatorPanel::on_cageKeyframeList_itemClicked(QListWidgetItem *item)
{
   std::string label = item->text().toStdString();
   std::istringstream iss(label);

   int keyFrameIndex;
   iss >> keyFrameIndex;
   setCageKeyframe(keyFrameIndex);
   Controller * c = Controller::get();
   c->glCanvas->runSkinningPipeline();
   c->glCanvas->updateGL();
}

void AsyncAnimatorPanel::on_bake_clicked()
{
   Controller * controller = Controller::get();
   if(controller->isCharacterLoaded)
   {
      const std::vector<double> ck = controller->asyncAnimator->getCageKeyframeTimeVector();
      const std::vector<double> sk = controller->asyncAnimator->getSkelKeyframeTimeVector();

      double cageMaxTime = ck[ck.size()-1];
      double skelMaxTime = sk[sk.size()-1];
      double maxTime = std::max(cageMaxTime, skelMaxTime);

      //QTime t;

      if((controller->asyncAnimator->getNumberOfSkelKeyframes()>0) ||
            (controller->asyncAnimator->getNumberOfCageKeyframes()>0)  )
      {
         std::string filepath;

         if (openFileSaveDialog(filepath, "Save Character", "3D Meshes (*.obj *.ply)"))
         {
            std::string filename = cinolib::get_file_name(filepath,false);
            filepath = cinolib::get_file_path(filepath);
            double timelapseInterval = 0.013;
            //t.start();
            std::vector<double> keyTimes;

            for(double t=0.0; t < maxTime; t+=timelapseInterval)
            {
               keyTimes.push_back(t);
            }

            controller->asyncAnimator->setSkeletonKeyframeIndex(0);
            controller->asyncAnimator->setCageKeyframeIndex(0);

            for(double t:keyTimes)
            {
               controller->asyncAnimator->setSkeletonFrame(t);
               controller->asyncAnimator->setCageFrame(t);
               controller->glCanvas->runSkinningPipeline();

               std::vector<double> v = controller->character->getVerticesVector();
               std::vector<int>    f = controller->character->getTrianglesVector();

               std::string finalName = (filepath+filename+"_"+std::to_string(t)+".obj");
               saveOBJ(finalName.c_str(), v, f);
               std::cout << finalName << " " << t << std::endl;
            }
         }
      }
   }
}


void AsyncAnimatorPanel::on_bakePic_clicked()
{
#ifdef CUSTOMSNAPSHOTQGL
   Controller * controller = Controller::get();
   if(controller->isCharacterLoaded)
   {
      const std::vector<double> ck = controller->asyncAnimator->getCageKeyframeTimeVector();
      const std::vector<double> sk = controller->asyncAnimator->getSkelKeyframeTimeVector();

      double cageMaxTime = ck[ck.size()-1];
      double skelMaxTime = sk[sk.size()-1];
      double maxTime = std::max(cageMaxTime, skelMaxTime);

      if((controller->asyncAnimator->getNumberOfSkelKeyframes()>0) ||
            (controller->asyncAnimator->getNumberOfCageKeyframes()>0)  )
      {
         std::string filepath;

         if (openFileSaveDialog(filepath, "Save Character", "images (*.jpg)"))
         {
            std::string filename = cinolib::get_file_name(filepath,false);
            filepath = cinolib::get_file_path(filepath);
            controller->glCanvas->openSnapshotFormatDialog();
            double timelapseInterval = 0.01;
            std::vector<double> keyTimes;

            for(double t=0.0; t < maxTime; t+=timelapseInterval)
            {
               keyTimes.push_back(t);
            }
            controller->asyncAnimator->setSkeletonKeyframeIndex(0);
            controller->asyncAnimator->setCageKeyframeIndex(0);

            for(double t:keyTimes)
            {
               //controller->asyncAnimator->updateFrame();
               controller->asyncAnimator->setSkeletonFrame(t);
               controller->asyncAnimator->setCageFrame(t);
               controller->glCanvas->runSkinningPipeline();
               controller->glCanvas->refreshScene();
               std::string finalName = (filepath+filename+"_"+std::to_string(t)+".jpg");
               controller->glCanvas->saveImageSnapshot(QString(finalName.c_str()));
            }
         }
      }
   }
#endif
}

void AsyncAnimatorPanel::on_bakeRest_clicked()
{
   Controller * controller = Controller::get();
   if(controller->isCharacterLoaded)
   {
      const std::vector<double> ck = controller->asyncAnimator->getCageKeyframeTimeVector();
      const std::vector<double> sk = controller->asyncAnimator->getSkelKeyframeTimeVector();

      double cageMaxTime = ck[ck.size()-1];
      double skelMaxTime = sk[sk.size()-1];
      double maxTime = std::max(cageMaxTime, skelMaxTime);

      //QTime t;

      if((controller->asyncAnimator->getNumberOfSkelKeyframes()>0) ||
            (controller->asyncAnimator->getNumberOfCageKeyframes()>0)  )
      {
         std::string filepath;

         if (openFileSaveDialog(filepath, "Save Character", "3D Meshes (*.obj *.ply)"))
         {
            std::string filename = cinolib::get_file_name(filepath,false);
            filepath = cinolib::get_file_path(filepath);
            double timelapseInterval = 0.013;
            //t.start();
            std::vector<double> keyTimes;

            for(double t=0.0; t < maxTime; t+=timelapseInterval)
            {
               keyTimes.push_back(t);
            }

            controller->asyncAnimator->setSkeletonKeyframeIndex(0);
            controller->asyncAnimator->setCageKeyframeIndex(0);

            for(double t:keyTimes)
            {
               controller->asyncAnimator->setSkeletonFrame(t);
               controller->asyncAnimator->setCageFrame(t);
               controller->glCanvas->runSkinningPipeline();

               std::vector<double> v = controller->character->getRestPoseVerticesVector();
               std::vector<int>    f = controller->character->getTrianglesVector();

               std::string finalName = (filepath+filename+"_"+std::to_string(t)+".obj");
               saveOBJ(finalName.c_str(), v, f);
               std::cout << finalName << " " << t << std::endl;
            }
         }
      }
   }
}

void AsyncAnimatorPanel::on_bakePicRest_clicked()
{
#ifdef CUSTOMSNAPSHOTQGL
   Controller * controller = Controller::get();
   if(controller->isCharacterLoaded)
   {
      controller->restPoseCanvas->startAnimation();
      const std::vector<double> ck = controller->asyncAnimator->getCageKeyframeTimeVector();
      const std::vector<double> sk = controller->asyncAnimator->getSkelKeyframeTimeVector();
      double cageMaxTime = ck[ck.size()-1];
      double skelMaxTime = sk[sk.size()-1];
      double maxTime = std::max(cageMaxTime, skelMaxTime);
      if((controller->asyncAnimator->getNumberOfSkelKeyframes()>0) ||
            (controller->asyncAnimator->getNumberOfCageKeyframes()>0)  )
      {
         std::string filepath;

         if (openFileSaveDialog(filepath, "Save Character", "images (*.jpg)"))
         {
            std::string filename = cinolib::get_file_name(filepath,false);
            filepath = cinolib::get_file_path(filepath);
            controller->glCanvas->openSnapshotFormatDialog();
            double timelapseInterval = 0.013;
            std::vector<double> keyTimes;
            for(double t=0.0; t < maxTime; t+=timelapseInterval)
            {
               keyTimes.push_back(t);
            }
            controller->asyncAnimator->setSkeletonKeyframeIndex(0);
            controller->asyncAnimator->setCageKeyframeIndex(0);
            for(double t:keyTimes)
            {
               controller->asyncAnimator->setSkeletonFrame(t);
               controller->asyncAnimator->setCageFrame(t);
               controller->glCanvas->runSkinningPipeline();
               controller->glCanvas->refreshScene();
               std::string finalName = (filepath+filename+"_"+std::to_string(t)+".jpg");
               controller->restPoseCanvas->saveImageSnapshot(QString(finalName.c_str()));
            }
         }
      }
   }
#endif
}

void AsyncAnimatorPanel::on_horizontalSlider_sliderMoved(int position)
{
   Controller * controller = Controller::get();
   controller->asyncAnimator->setAnimationSpeed(static_cast<double>(position)/50.0);
}
