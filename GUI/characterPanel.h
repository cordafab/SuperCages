#ifndef CHARACTERPANEL_H
#define CHARACTERPANEL_H

#include <QDockWidget>

namespace Ui {
   class CharacterPanel;
}

class CharacterPanel : public QDockWidget
{
   Q_OBJECT

public:
   explicit CharacterPanel(QWidget *parent = nullptr);
   ~CharacterPanel();

private:
   Ui::CharacterPanel *ui;

private slots:
   void on_drawTrimeshCB_clicked(bool checked);
   void on_smoothColourRB_clicked(bool checked);
   void on_flatColourRB_clicked(bool checked);
   void on_wireColourRB_clicked(bool checked);
   void on_cageColourRB_clicked(bool checked);
   void on_skelColourRB_clicked(bool checked);
   void on_skelUpdtrColourRB_clicked(bool checked);
   void on_texturedRB_clicked(bool checked);
   void on_skelSkin_clicked();
   void on_cageSkin_clicked();
   void on_exportChar_clicked();
};

#endif // CHARACTERPANEL_H
