#include "restPoseCanvas.h"
#include "drawables/drawableCharacter.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"

#include "skinning/cageSkinning.h"
#include "skinning/skeletonSkinning.h"
#include "operators/skeletonUpdater.h"
#include "operators/cageUpdater.h"

#include "QGLViewer/camera.h"
#include "QGLViewer/manipulatedCameraFrame.h"

#include "GUI/glUtils.h"

#include "common/cageOperations.h"
#include "common/skeletonOperations.h"

#include "skinning/corSkinning.h"

#include <QMouseEvent>

RestPoseCanvas::RestPoseCanvas(QWidget *parent)
   : QGLViewer(parent)
{
   init();
}

void RestPoseCanvas::draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable (GL_LINE_SMOOTH);
   glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

   initLighting();
   initMaterial();
   initInverseMaterial();
   //setSingleLighting();
   //setMultiLighting();

   setBackgroundColor(customBackgroundColor);

   if(controller->isCharacterLoaded)
      controller->character->drawRest();
   if(controller->isCageLoaded)
      controller->cage->drawRest();
   if(controller->isSkeletonLoaded)
      controller->skeleton->drawRest();

   if (isSelectionRectangleActive)
   {
      startScreenCoordinatesSystem();
      drawSelectionRectangle(selectionRectangle.top(),
                             selectionRectangle.bottom(),
                             selectionRectangle.left(),
                             selectionRectangle.right());
      stopScreenCoordinatesSystem();
   }
}

void RestPoseCanvas::drawWithNames()
{
   if(controller->isCageLoaded)
      controller->cage->drawWithNamesRest();
}

void RestPoseCanvas::refreshScene()
{
   update();
}

void RestPoseCanvas::init()
{
   controller = Controller::get();
   pickerController = PickerController::get();

   fitScene();

   customBackgroundColor.setRgb(255,255,255);

   setFPSIsDisplayed(true);

   startAnimation();


   camera()->frame()->setSpinningSensitivity(100.0);
}

void RestPoseCanvas::fitScene()
{
   sceneCenter.set(0.0,0.0,0.0);
   sceneRadius = 0.00001;

   if(controller->isCharacterLoaded)
   {
      controller->character->sceneCenter(sceneCenter);
      controller->character->sceneRadius(sceneRadius);
   }

   setSceneCenter(qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
   setSceneRadius(sceneRadius);

   showEntireScene();
}

void RestPoseCanvas::animate()
{

}

// Customized mouse events
void RestPoseCanvas::mousePressEvent(QMouseEvent* e)
{
   if ((e->modifiers()==Qt::ShiftModifier) && (e->buttons() & Qt::LeftButton)){
      interactionMode = SELECT;
   } else
      if ((e->modifiers()==Qt::ShiftModifier) && (e->buttons() & Qt::RightButton)){
         interactionMode = DESELECT;
      } else
         if ( e->modifiers()==Qt::ControlModifier ){
            interactionMode = DEFORM;
         }

   if ((interactionMode == SELECT) || (interactionMode == DESELECT))
   {
      selectionRectangle = QRect(e->pos(), e->pos());
      isSelectionRectangleActive = true;
      } else
   if (interactionMode == DEFORM)
   {
      //ClickConverter Initialization
      qglviewer::Vec qglCameraPosition = camera()->position();
      qglviewer::Vec qglCameraDirection = camera()->viewDirection();
      cg3::Vec3d cameraPosition(qglCameraPosition.x, qglCameraPosition.y, qglCameraPosition.z);
      cg3::Vec3d cameraDirection(qglCameraDirection.x, qglCameraDirection.y, qglCameraDirection.z);
      GLfloat projectionMatrix[16];
      GLfloat viewMatrix[16];
      camera()->getProjectionMatrix(projectionMatrix);
      camera()->getModelViewMatrix(viewMatrix);
      clickConverter.init(e->x(),
                          e->y(),
                          cameraPosition,
                          cameraDirection,
                          sceneCenter,
                          projectionMatrix,
                          viewMatrix,
                          width(),
                          height());

      if (e->buttons() & Qt::RightButton)
      {

      } else
      if (e->buttons() & Qt::LeftButton)
      {

      }
   }
   else
      QGLViewer::mousePressEvent(e);
}

void RestPoseCanvas::mouseMoveEvent(QMouseEvent* e)
{
   if ((interactionMode == SELECT) || (interactionMode == DESELECT))
   {
      selectionRectangle.setBottomRight(e->pos());
      update();
      } else
   if ( interactionMode == DEFORM )
   {

      if (e->buttons() & Qt::RightButton)
      {
         clickConverter.updateMouseMovement(e->x(),e->y());
         computePickableObjectsTranslation();
      } else
      if (e->buttons() & Qt::LeftButton)
      {
         clickConverter.updateMouseMovement(e->x(),e->y());
         computePickableObjectsRotation();
      }


      //Cage Skinning
      if(controller->isCageSkinningInitialized &&
         controller->cage->refreshCharacterPose())
      {
         controller->cageSkinning->deform();

         if(controller->skeletonSkinning == controller->cor)
         {
            controller->cor->updateCoRs();
         }

         if(controller->isSkeletonUpdaterActive)
         {
            controller->skeletonUpdater->updatePosition();
         }

         controller->skeletonSkinning->deform();
         controller->cageUpdater->updatePosition();
         //controller->cageSkinning->deform();


         controller->character->updateNormals();
         controller->character->updateCutVerticesPosition();
      }
      update();
   }
   else
      QGLViewer::mouseMoveEvent(e);
}

void RestPoseCanvas::mouseReleaseEvent(QMouseEvent* e)
{
   if ((interactionMode == SELECT) || (interactionMode == DESELECT))
   {
      selectionRectangle = selectionRectangle.normalized();
      int width = selectionRectangle.width();
      if(width>1)
          setSelectRegionWidth(width);
      else
          setSelectRegionWidth(1);
      int height = selectionRectangle.height();
      if(height>1)
          setSelectRegionHeight(height);
      else
          setSelectRegionHeight(1);
      select(selectionRectangle.center());
      isSelectionRectangleActive = false;
      update();
   }
   else if ( interactionMode==DEFORM )
   {
      interactionMode = CAMERA;

      if(controller->isSkeletonSkinningInitialized &&
         controller->skeleton->refreshCharacterPoseIsNeeded())
      {
         controller->skeleton->characterPoseRefreshed();
      }

      if(controller->isCageSkinningInitialized &&
            controller->cage->refreshCharacterPose())
      {
         controller->cage->characterPoseRefreshed();
      }

      if(controller->isSkeletonSkinningInitialized &&
         controller->isCageSkinningInitialized        )
      {
         //controller->cage->updateCurrentPoseNormals();
         //controller->cage->updateFrames();
      }

      update();
   }
   else
      QGLViewer::mouseReleaseEvent(e);
}

void RestPoseCanvas::wheelEvent(QWheelEvent *e)
{
   if(e->modifiers()==Qt::ControlModifier    &&
      controller->isCageSkinningInitialized  )
   {
      computePickableObjectsScaling(e->delta());
      controller->cageSkinning->deform();

      if(controller->skeletonSkinning == controller->cor)
      {
         controller->cor->updateCoRs();
      }

      if(controller->isSkeletonUpdaterActive)
      {
         controller->skeletonUpdater->updatePosition();

      }

      controller->skeletonSkinning->deform();
      controller->cageUpdater->updatePosition();
      controller->cage->characterPoseRefreshed();
      controller->character->updateNormals();
      //controller->cage->updateCurrentPoseNormals();
      //controller->cage->updateFrames();
      controller->character->updateCutVerticesPosition();
      update();
   }
   else
      QGLViewer::wheelEvent(e);
}

void RestPoseCanvas::keyPressEvent(QKeyEvent *e)
{
   switch (e->key())
      {

         //case Qt::Key_Enter:
         //case Qt::Key_Return:
         case Qt::Key_Space:

         break;

         default:
            QGLViewer::keyPressEvent(e);
   }
}

void RestPoseCanvas::endSelection(const QPoint &)
{
   glFlush();
   GLint nbHits = glRenderMode(GL_RENDER);
   if (nbHits > 0)
   {
      // (selectBuffer())[4*i+3] is the id pushed on the stack.
      for (int i=0; i<nbHits; ++i)
      {
         int pickedIndex = (selectBuffer())[4*i+3];
         switch(interactionMode)
         {
            case SELECT:
               pickerController->getObject(pickedIndex)->selectObject(pickedIndex);
               break;
            case DESELECT:
               pickerController->getObject(pickedIndex)->deselectObject(pickedIndex);
               break;
            default: break;
         }
      }

      if(interactionMode==SELECT || interactionMode==DESELECT){
         if(controller->isCageWeightsRenderActive)
            updateCageInfluenceTexture();
         else if(controller->isSkeletonWeightsRenderActive)
            updateSkeletonInfluenceTexture(true);
         else if(controller->isSkeletonUpdaterWeightsRenderActive)
            updateSkeletonUpdaterInfluenceTexture();
      }
   }
   interactionMode = CAMERA;
}

//Deformation of selected objects
void RestPoseCanvas::computePickableObjectsTranslation()
{
   cg3::Vec3d delta;
   clickConverter.getTranslation(delta);

   //controller->skeleton->translateRest(delta);
   controller->cage->translateRest(delta);

}

void RestPoseCanvas::computePickableObjectsRotation()
{
   cg3::dQuaternion rotation;
   clickConverter.getRotation(rotation, sceneRadius);

   //controller->skeleton->rotateRest(rotation);
   controller->cage->rotateRest(rotation);
}

void RestPoseCanvas::computePickableObjectsScaling(int direction)
{

   controller->cage->scaleRest(direction);

}
