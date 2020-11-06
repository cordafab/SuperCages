#ifndef ASYNCANIMATORPANEL_H
#define ASYNCANIMATORPANEL_H

#include <QDockWidget>
#include <QListWidgetItem>

namespace Ui {
   class AsyncAnimatorPanel;
}

class AsyncAnimatorPanel : public QDockWidget
{
   Q_OBJECT

public:
   explicit AsyncAnimatorPanel(QWidget *parent = nullptr);
   ~AsyncAnimatorPanel();

   void updateKeyframeList();

private slots:
   void on_addSkelKeyframe_clicked();

   void on_addCageKeyframe_clicked();

   void on_editSkelTime_clicked();

   void on_editCageTime_clicked();

   void on_deleteSkelKeyframe_clicked();

   void on_deleteCageKeyframe_clicked();

   void on_nextSkelKeyframe_clicked();

   void on_nextCageKeyframe_clicked();

   void on_loadSkelAnimation_clicked();

   void on_loadCageAnimation_clicked();

   void on_saveSkelAnimation_clicked();

   void on_saveCageAnimation_clicked();

   void on_skelKeyframeList_itemClicked(QListWidgetItem *item);

   void on_cageKeyframeList_itemClicked(QListWidgetItem *item);

   void on_bake_clicked();

   void on_bakePic_clicked();

   void on_bakeRest_clicked();

   void on_bakePicRest_clicked();

   void on_horizontalSlider_sliderMoved(int position);

private:
   Ui::AsyncAnimatorPanel *ui;
};

#endif // ASYNCANIMATORPANEL_H
