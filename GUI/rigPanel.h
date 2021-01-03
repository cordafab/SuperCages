#ifndef FBXIMPORTPANEL_H
#define FBXIMPORTPANEL_H

#include <QDockWidget>

#include "drawables/drawableSkeleton.h"

namespace Ui {
   class rigPanel;
}

class RigPanel : public QDockWidget
{
   Q_OBJECT

public:
   explicit RigPanel(QWidget *parent = nullptr);
   ~RigPanel();

private slots:
   void on_loadRig_clicked();

private:
   Ui::rigPanel *ui;
};

#endif // FBXIMPORTPANEL_H
