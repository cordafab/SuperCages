#include "rigPanel.h"
#include "ui_rigPanel.h"

#include "common/rigOperations.h"

RigPanel::RigPanel(QWidget *parent) :
   QDockWidget(parent),
   ui(new Ui::rigPanel)
{
   ui->setupUi(this);

   ui->loadRig->setIcon(QIcon(QPixmap(":/GUI/icons/fbx.png")));
   ui->loadRig->setStyleSheet("padding:5px; text-align:left;");
   ui->loadRig->setIconSize(QSize(18, 18));
}

RigPanel::~RigPanel()
{
   delete ui;
}

void RigPanel::on_loadRig_clicked()
{
   importRig();
}
