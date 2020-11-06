#ifndef TOOLSPANEL_H
#define TOOLSPANEL_H

#include <QDockWidget>

namespace Ui {
   class ToolsPanel;
}

class ToolsPanel : public QDockWidget
{
   Q_OBJECT

public:
   explicit ToolsPanel(QWidget *parent = nullptr);
   ~ToolsPanel();

   void updateSkelDeformationButton();
   void updateCageDeformationButton();
   void updateCageSkelUpdaterButton();

private slots:
   void on_skelDeformation_clicked();

   void on_cageDeformation_clicked();

   void on_saveCamera_clicked();

   void on_restoreCamera_clicked();

   void on_cageUpdater_clicked();

   void on_skelUpdater_clicked();

   void on_exportCamera_clicked();

   void on_importCamera_clicked();

private:
   Ui::ToolsPanel *ui;
};

#endif // TOOLSPANEL_H
