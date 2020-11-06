/**
   @author     Fabrizio Corda (cordafab@gmail.com)
   @copyright  Fabrizio Corda, 2017
**/

#ifndef __APPLE__
#define GLEW_STATIC 1
#include <GL/glew.h>
#endif

#include <QApplication>
#include <QStyleFactory>

#include "environment.h"

#include "GUI/mainWindow.h"

int main(int argc, char* argv[])
{

   QApplication app(argc,argv);
   QApplication::setStyle(QStyleFactory::create("Fusion"));

   Environment e;

   MainWindow gui;
   gui.show();

   return app.exec();
}
