#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <qsplitter.h>

#include "common/animatorOperations.h"

#include "GUI/glCanvas.h"
#include "GUI/rigPanel.h"
#include "GUI/characterPanel.h"
#include "GUI/cagePanel.h"
#include "GUI/skeletonPanel.h"
#include "GUI/asyncAnimatorPanel.h"
#include "GUI/toolsPanel.h"
#include "GUI/restPoseCanvas.h"
#include "GUI/qtUtils.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent),
     ui(new Ui::MainWindow)
{
   ui->setupUi(this);

   controller = Controller::get();

   controller->mainWindow = this;

   QSplitter *hSplit = new QSplitter(Qt::Horizontal);

   //configure glCanvas
   controller->glCanvas = new GlCanvas(hSplit);
   controller->glCanvas->setObjectName(QStringLiteral("GlCanvas"));

   //configure restPoseCanvas
   controller->restPoseCanvas = new RestPoseCanvas(hSplit);
   controller->restPoseCanvas->setObjectName(QStringLiteral("RestPoseCanvas"));

   // Set main QSplitter as the main widget.
   ui->verticalLayout->addWidget(hSplit);

   //configure fbxImportPanel
   if(!controller->rigPanel)
   {
      controller->rigPanel = new RigPanel(this);
   }
   controller->rigPanel->show();
   addDockWidget(Qt::LeftDockWidgetArea, controller->rigPanel);

   //configure characterPanel
   if(!controller->characterPanel)
   {
      controller->characterPanel = new CharacterPanel(this);
   }
   controller->characterPanel->show();
   addDockWidget(Qt::LeftDockWidgetArea, controller->characterPanel);

   //configure cagePanel
   if(!controller->cagePanel)
   {
      controller->cagePanel = new CagePanel(this);
   }
   controller->cagePanel->show();
   addDockWidget(Qt::LeftDockWidgetArea, controller->cagePanel);

   //configure skeletonPanel
   if(!controller->skeletonPanel)
   {
      controller->skeletonPanel = new SkeletonPanel(this);
   }
   controller->skeletonPanel->show();
   addDockWidget(Qt::LeftDockWidgetArea, controller->skeletonPanel);

   //configure asyncAnimatorPanel
   if(!controller->asyncAnimatorPanel)
   {
      controller->asyncAnimatorPanel = new AsyncAnimatorPanel(this);
   }
   controller->asyncAnimatorPanel->show();
   addDockWidget(Qt::LeftDockWidgetArea, controller->asyncAnimatorPanel);

   //configure toolsPanel
   if(!controller->toolsPanel)
   {
      controller->toolsPanel = new ToolsPanel(this);
   }
   controller->toolsPanel->show();
   addDockWidget(Qt::RightDockWidgetArea, controller->toolsPanel);

   updateGUI();

   //tabifyDockWidget(controller->rigPanel, controller->characterPanel);
   tabifyDockWidget(controller->characterPanel, controller->cagePanel);
   tabifyDockWidget(controller->cagePanel, controller->skeletonPanel);
   controller->characterPanel->raise();

   showMaximized();
}

MainWindow::~MainWindow()
{
   delete ui;
}
