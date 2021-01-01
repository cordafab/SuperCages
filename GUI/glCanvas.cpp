#include "glCanvas.h"

#include <math.h>
#include <algorithm>

#include "controller.h"
#include "drawables/drawableCharacter.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"

#include "skinning/linearBlendSkinning.h"
#include "skinning/cageSkinning.h"

#include "common/cageOperations.h"
#include "common/skeletonOperations.h"

#include "GUI/restPoseCanvas.h"

#include <QMouseEvent>
#include "QGLViewer/camera.h"
#include "GUI/glUtils.h"
#include "Eigen/Eigen"
#include "QGLViewer/vec.h"
#include "geom/plane.h"
#include "geom/quaternion.h"
#include "QGLViewer/manipulatedCameraFrame.h"
#include "operators/cageUpdater.h"
#include "operators/skeletonUpdater.h"
#include "operators/cageReverser.h"
#include "animation/asyncAnimator.h"
#include "skinning/corSkinning.h"

#include <iostream>
#include <chrono>
#include <QElapsedTimer>


GlCanvas::GlCanvas(QWidget *parent)
   : QGLViewer(parent)
{
   init();
}

void GlCanvas::init()
{
   controller = Controller::get();
   pickerController = PickerController::get();

   fitScene();

   interactionMode = CAMERA;
   isSelectionRectangleActive = false;

   customBackgroundColor.setRgb(255,255,255);

   setFPSIsDisplayed(true);

   setAnimationPeriod(0);
   startAnimation();

   camera()->frame()->setSpinningSensitivity(100.0);

   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glEnable (GL_BLEND);
   //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   //glEnable (GL_LINE_SMOOTH);
   //glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

   //initLighting();
   //initMaterial();
   //initInverseMaterial();
   //setSingleLighting();
   //setMultiLighting();

}

void GlCanvas::pushDrawableObject(const DrawableObject * object)
{
   drawableObjects.push_back(object);
   fitScene();
   refreshScene();
}

void GlCanvas::removeDrawableObject(const DrawableObject * object)
{
   std::vector<const DrawableObject *>::iterator itr = drawableObjects.begin();
   while (itr != drawableObjects.end()) {
      if (*itr == object) {
         itr = drawableObjects.erase(itr);
      } else {
         ++itr;
      }
   }
   refreshScene();
}

void GlCanvas::pushPickableObject(PickableObject * object)
{
   pickableObjects.push_back(object);
   refreshScene();
}

void GlCanvas::removePickableObject(PickableObject *object)
{
   std::vector<PickableObject *>::iterator itr = pickableObjects.begin();
   while (itr != pickableObjects.end()) {
      if (*itr == object) {
         itr = pickableObjects.erase(itr);
      } else {
         ++itr;
      }
   }
   refreshScene();
}

void GlCanvas::draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   setBackgroundColor(customBackgroundColor);

   for(unsigned long i=0; i<drawableObjects.size(); ++i)
   {
      drawableObjects[i]->draw();
   }

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

void GlCanvas::drawWithNames()
{
   for(unsigned long i=0; i<pickableObjects.size(); ++i)
   {
      pickableObjects[i]->drawWithNames();
   }
}

void GlCanvas::animate()
{
   if((controller->isAnimatorActivated))
   {
      if((controller->asyncAnimator->getNumberOfSkelKeyframes()>0) ||
            (controller->asyncAnimator->getNumberOfCageKeyframes()>0)  )
      {
         controller->asyncAnimator->updateFrame();
         runSkinningPipeline();
      }
   }
}

void GlCanvas::refreshScene()
{
   if(controller->restPoseCanvas)
   {
      controller->restPoseCanvas->refreshScene();
   }
   update();
}

void GlCanvas::runSkinningPipeline()
{
   //Skinning
   if(controller->isCageSkinningInitialized &&
      controller->isSkeletonSkinningInitialized)
   {
      controller->cageSkinning->deform();

      if(controller->isSkeletonUpdaterActive)
      {
         controller->skeletonUpdater->updatePosition();
      }
   }

   if(controller->isSkeletonSkinningInitialized)
   {
      controller->skeleton->updateGlobalT();
      controller->skeletonSkinning->deform();
      controller->character->updateNormals();
      controller->character->updateCutVerticesPosition();
   }

   if(controller->isCageUpdaterInitialized && controller->isCageUpdaterActive)
   {
      controller->cageUpdater->updatePosition();
   }

}

void GlCanvas::fitScene()
{
   sceneCenter.set(0.0,0.0,0.0);
   sceneRadius = 0.00001;

   int numberOfValidObjectCentroid = 0;

   for(ulong i=0; i<drawableObjects.size(); ++i)
   {
      const DrawableObject * obj = drawableObjects[i];

      cg3::Vec3d objSceneCenter;
      double objSceneRadius;

      if(i==0)
      {
         if(obj->sceneCenter(objSceneCenter))
         {
            sceneCenter = objSceneCenter;
            numberOfValidObjectCentroid++;
         }
      }
      else
      {
         if(obj->sceneCenter(objSceneCenter))
         {
            sceneCenter += objSceneCenter;
            numberOfValidObjectCentroid++;
         }
      }

      if(obj->sceneRadius(objSceneRadius))
      {
         sceneRadius  = std::max(sceneRadius, objSceneRadius);
      }
   }

   if(numberOfValidObjectCentroid > 1)
   {
      sceneCenter /= static_cast<double>(numberOfValidObjectCentroid);
   }

   setSceneCenter(qglviewer::Vec(sceneCenter.x(), sceneCenter.y(), sceneCenter.z()));
   setSceneRadius(sceneRadius);

   showEntireScene();

   if(controller->restPoseCanvas)
   {
      controller->restPoseCanvas->fitScene();
   }
}

void GlCanvas::saveCamera()
{
   savedCameraPosition = camera()->position();
   savedCameraOrientation = camera()->orientation();
   savedCameraUpVector = camera()->upVector();
   savedCameraViewDirection = camera()->viewDirection();
}

void GlCanvas::restoreCamera()
{
   camera()->setPosition(savedCameraPosition);
   camera()->setOrientation(savedCameraOrientation);
   camera()->setUpVector(savedCameraUpVector);
   camera()->setViewDirection(savedCameraViewDirection);

   controller->restPoseCanvas->camera()->setPosition(savedCameraPosition);
   controller->restPoseCanvas->camera()->setOrientation(savedCameraOrientation);
   controller->restPoseCanvas->camera()->setUpVector(savedCameraUpVector);
   controller->restPoseCanvas->camera()->setViewDirection(savedCameraViewDirection);
   update();
   controller->restPoseCanvas->update();
}

void GlCanvas::getCameraParams(std::vector<double> & params)
{
   params.push_back(savedCameraPosition.x);
   params.push_back(savedCameraPosition.y);
   params.push_back(savedCameraPosition.z);
   params.push_back(savedCameraOrientation[0]);
   params.push_back(savedCameraOrientation[1]);
   params.push_back(savedCameraOrientation[2]);
   params.push_back(savedCameraOrientation[3]);
   params.push_back(savedCameraUpVector.x);
   params.push_back(savedCameraUpVector.y);
   params.push_back(savedCameraUpVector.z);
   params.push_back(savedCameraViewDirection.x);
   params.push_back(savedCameraViewDirection.y);
   params.push_back(savedCameraViewDirection.z);
}

void GlCanvas::setCameraParams(const std::vector<double> & params)
{
   savedCameraPosition.setValue(params[0],params[1],params[2]);
   savedCameraOrientation[0] = params[3];
   savedCameraOrientation[1] = params[4];
   savedCameraOrientation[2] = params[5];
   savedCameraOrientation[3] = params[6];
   savedCameraUpVector.setValue(params[7],params[8],params[9]);
   savedCameraViewDirection.setValue(params[10],params[11],params[12]);
}

// Customized mouse events
void GlCanvas::mousePressEvent(QMouseEvent* e)
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

void GlCanvas::mouseMoveEvent(QMouseEvent* e)
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
            translatePicableObjects();
         } else
            if (e->buttons() & Qt::LeftButton)
            {
               clickConverter.updateMouseMovement(e->x(),e->y());
               rotatePickableObjects();
            }


         //Skeleton Skinning
         if(controller->isSkeletonSkinningInitialized &&
            controller->skeleton->refreshCharacterPoseIsNeeded())
         {

            controller->skeletonSkinning->deform();

            if(controller->isCageUpdaterInitialized && controller->isCageUpdaterActive)
            {
               controller->cageReverser->skeletonEdited();
               controller->cageUpdater->updatePosition();
            }

            controller->character->updateNormals();
         }


         //Cage Skinning
         if(controller->isCageSkinningInitialized &&
            controller->cage->refreshCharacterPose())
         {

            controller->cageReverser->propagateToRest();
            controller->cageSkinning->deform();

            if(controller->skeletonSkinning == controller->cor)
            {
               controller->cor->updateCoRs();
            }

            if(controller->isSkeletonUpdaterActive)
            {
               controller->skeletonUpdater->updatePosition();
               controller->skeleton->updateGlobalT();
            }

            controller->skeletonSkinning->deform();
            controller->cageUpdater->updatePosition();

            controller->character->updateNormals();
            controller->character->updateCutVerticesPosition();
         }

         update();
      }
      else
         QGLViewer::mouseMoveEvent(e);
}

void GlCanvas::mouseReleaseEvent(QMouseEvent* e)
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
         //controller->character->updateNormals();
      }

      if(controller->isCageSkinningInitialized &&
            controller->cage->refreshCharacterPose())
      {
         //controller->cageSkinning->deform();
         controller->skeletonSkinning->deform();
         //controller->cageUpdater->updatePosition();

         controller->cage->characterPoseRefreshed();
         controller->character->updateNormals();
      }

      update();
   }
   else
      QGLViewer::mouseReleaseEvent(e);
}

void GlCanvas::wheelEvent(QWheelEvent *e)
{
   if(e->modifiers()==Qt::ControlModifier    &&
         controller->isCageSkinningInitialized   )
   {
      //potrebbe creare bug se uso la rotellina mentre ruoto skel/cage
      scalePickableObjects(e->delta());

      controller->cageReverser->propagateToRest();
      controller->cageSkinning->deform();

      if(controller->skeletonSkinning == controller->cor)
      {
         controller->cor->updateCoRs();
      }

      if(controller->isSkeletonUpdaterActive)
      {
         controller->skeletonUpdater->updatePosition();
         controller->skeleton->updateGlobalT();
      }

      controller->skeletonSkinning->deform();
      controller->cageUpdater->updatePosition();
      controller->character->updateNormals();
      controller->character->updateCutVerticesPosition();

      controller->cage->characterPoseRefreshed();
      controller->character->updateNormals();

      update();
   }
   else
      QGLViewer::wheelEvent(e);
}

void GlCanvas::keyPressEvent(QKeyEvent *e)
{
   switch (e->key())
   {
   case Qt::Key_P:
      controller->asyncAnimator->rewindAnimator();
      if(controller->isCageSkinningInitialized) controller->cageReverser->skeletonEdited();
      if(controller->isAnimatorActivated)
      {
         controller->skeleton->resetRootMotion();
         controller->skeletonSkinning->deform();
         if(controller->isCageSkinningInitialized) controller->cageUpdater->updatePosition();
      }
      controller->isAnimatorActivated = !(controller->isAnimatorActivated);
      break;
   case Qt::Key_Enter:
   case Qt::Key_Return:
   case Qt::Key_Space:

      break;
   default:
      QGLViewer::keyPressEvent(e);
   }
}


//Selection
void GlCanvas::endSelection(const QPoint&)
{
   glFlush();
   GLint nbHits = glRenderMode(GL_RENDER);
   if (nbHits > 0)
   {
      // (selectBuffer())[4*i+3] is the id pushed on the stack.
      for (int i=0; i<nbHits; ++i)
      {
         unsigned long pickedIndex = (selectBuffer())[4*i+3];
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
void GlCanvas::translatePicableObjects()
{
   cg3::Vec3d delta;
   clickConverter.getTranslation(delta);

   for(unsigned long i=0; i<pickableObjects.size(); ++i)
   {
      pickableObjects[i]->translate(delta);
   }
}

void GlCanvas::rotatePickableObjects()
{
   cg3::dQuaternion rotation;
   clickConverter.getRotation(rotation, sceneRadius);

   for(unsigned long i=0; i<pickableObjects.size(); ++i)
   {
      pickableObjects[i]->rotate(rotation);
   }
}

void GlCanvas::scalePickableObjects(int scaleFactor)
{
   for(unsigned long i=0; i<pickableObjects.size(); ++i)
   {
      pickableObjects[i]->scale(scaleFactor);
   }
}
