#ifndef DEBUGCANVAS_H
#define DEBUGCANVAS_H

#include <QGLViewer/qglviewer.h>
#include "GUI/clickConverter.h"

#include "controller.h"
#include "drawables/pickerController.h"

#include "geom/vec3.h"

class RestPoseCanvas
      : public QGLViewer
{
public:
   RestPoseCanvas(QWidget * parent);

   void draw();

   void animate();

   void refreshScene();
   void fitScene();

   void drawWithNames();
protected :

   Controller * controller;
   PickerController * pickerController;

   cg3::Vec3d sceneCenter;
   double sceneRadius;

   enum InteractionMode { CAMERA, SELECT, DESELECT, DEFORM };
   InteractionMode interactionMode;

   void init();

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

   //Utilities for rotations
   cg3::Vec3d rotationAxis;

   //Deformation of selected objects
   void computePickableObjectsTranslation();
   void computePickableObjectsRotation();
   void computePickableObjectsScaling(int direction);

   QColor customBackgroundColor;
};

#endif // DEBUGCANVAS_H
