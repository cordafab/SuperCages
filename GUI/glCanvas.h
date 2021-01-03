#ifndef GLCANVAS_H
#define GLCANVAS_H


#include "controller.h"

#include "drawables/pickerController.h"

#include "geom/vec3.h"
#include "geom/ray.h"
#include "geom/plane.h"

#include "GUI/clickConverter.h"

#include <QGLViewer/qglviewer.h>
#include "glm/glm.hpp"

class GlCanvas
      : public QGLViewer
{
public :

   GlCanvas(QWidget * parent);

   void draw();
   void drawWithNames();

   void animate();

   void pushDrawableObject(const DrawableObject * object);
   void removeDrawableObject(const DrawableObject * object);

   void pushPickableObject(PickableObject *object);
   void removePickableObject(PickableObject *object);

   void refreshScene();
   void runSkinningPipeline();
   void fitScene();

   //Utilities for Camera saving
   void saveCamera();
   void restoreCamera();
   void getCameraParams(std::vector<double> & params);
   void setCameraParams(const std::vector<double> & params);


protected :

   Controller * controller;
   PickerController * pickerController;

   std::vector<const DrawableObject *> drawableObjects;
   std::vector<PickableObject *> pickableObjects;

   cg3::Vec3d sceneCenter;
   double sceneRadius;

   enum InteractionMode { CAMERA, SELECT, DESELECT, DEFORM };
   InteractionMode interactionMode;

   void init();

   QColor customBackgroundColor;

   //Mouse Customization
   void mousePressEvent(QMouseEvent *e);
   void mouseMoveEvent(QMouseEvent *e);
   void mouseReleaseEvent(QMouseEvent *e);
   void wheelEvent(QWheelEvent *e);

   //Keyboard Customization
   void keyPressEvent(QKeyEvent *e);

   //Selection
   void endSelection(const QPoint &);

   //Stuff for the selection rectangle
   QRect selectionRectangle;
   bool isSelectionRectangleActive;

   //Stuff for mouse spatial interaction
   ClickConverter clickConverter;

   //Deformation of selected objects
   void translatePicableObjects();
   void rotatePickableObjects();
   void scalePickableObjects(int scaleFactor);

   //Utilities for Camera saving
   qglviewer::Vec savedCameraPosition;
   qglviewer::Quaternion savedCameraOrientation;
   qglviewer::Vec savedCameraUpVector;
   qglviewer::Vec savedCameraViewDirection;

};

#endif // GLCANVAS_H
