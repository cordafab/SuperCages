#ifndef SKELETONPANEL_H
#define SKELETONPANEL_H

#include <QDockWidget>

namespace Ui {
   class SkeletonPanel;
}

class SkeletonPanel : public QDockWidget
{
   Q_OBJECT

public:
   explicit SkeletonPanel(QWidget *parent = nullptr);
   ~SkeletonPanel();

private slots:
   void on_drawSkelCB_clicked(bool checked);

   void on_lbsRB_toggled(bool checked);

   void on_dqsRB_toggled(bool checked);

   void on_corRB_toggled(bool checked);

private:
   Ui::SkeletonPanel *ui;
};

#endif // SKELETONPANEL_H
