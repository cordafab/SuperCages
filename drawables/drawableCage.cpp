#include "drawableCage.h"

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include "GUI/glUtils.h"
#include <vector>

//DEBUG FOR FRAME FLIP
//#include "cinolib/simplicial_map.h"
//#include "cinolib/matrix.h"


DrawableCage::DrawableCage()
{

}

DrawableCage::DrawableCage(const std::vector<double> & vertices ,
                           const std::vector<int>    & tris     )
{
    create(vertices, tris);
}

bool DrawableCage::create(const std::vector<double> & vertices,
                          const std::vector<int>    & tris     )
{
    clear();

    Cage::create(vertices, tris);

    init();

    return true;
}

DrawableCage::~DrawableCage()
{
    clear();
}

void DrawableCage::init()
{
   type           = CAGE;
   drawMode       = DRAW_CAGE | DRAW_VERTICES  | DRAW_WIREFRAME;// | DRAW_FRAMES;

   //initialize selectedVertices vector with false (all deselected)
   isVertexSelected.resize(originalRestPose.getNumVertices(), false);

   pickerController = PickerController::get();

   //assign PickableIndexes to every vertex
   for(ulong i=0; i<originalRestPose.getNumVertices(); ++i)
   {
      int pickableIndex = pickerController->generateIndex(this);
      pickableIndex2Vertex[pickableIndex] = i;
      vertex2PickableIndex[i] = pickableIndex;
   }
}

void DrawableCage::clear()
{
    Cage::clear();

    for(ulong i=0; i<originalRestPose.getNumVertices(); ++i)
    {
        pickerController->removeIndex(vertex2PickableIndex[i]);
    }

    drawMode = 0;
    _refreshCharacterPose = false;

    pickerController = nullptr;
    pickableIndex2Vertex.clear();
    vertex2PickableIndex.clear();
    isVertexSelected.clear();
    selectedVertices.clear();
}

void DrawableCage::draw() const
{
   if (drawMode & DRAW_CAGE)
   {
      const std::vector<double> & currentPoseVertices  = currentPose.getVerticesVector();
      const std::vector<int>    & currentPoseTriangles = currentPose.getTrianglesVector();

      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, currentPoseVertices.data());

         glLineWidth(2.0);
         glColor4f(0.20f, 0.20f, 0.20f, 0.8f);
         glDrawElements(GL_TRIANGLES, currentPoseTriangles.size(), GL_UNSIGNED_INT, currentPoseTriangles.data());

         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      double radius = currentPose.getBoundingBox().diagonal()/230.0;

      if(drawMode & DRAW_VERTICES)
      {
         setSingleLighting();
         for(ulong i=0; i<currentPose.getNumVertices(); ++i)
         {
            if (isVertexSelected[i])
               drawSphere(currentPoseVertices[i*3+0], currentPoseVertices[i*3+1], currentPoseVertices[i*3+2], radius, 0.905, 0.305, 0.305);
            else
               drawSphere(currentPoseVertices[i*3+0], currentPoseVertices[i*3+1], currentPoseVertices[i*3+2], radius, 0.99, 0.85, 0.39);
         }
      }
   }
}

void DrawableCage::drawRest() const
{
   if (drawMode & DRAW_CAGE)
   {

      const std::vector<double> & restPoseVertices  = restPose.getVerticesVector();
      const std::vector<int>    & restPoseTriangles = restPose.getTrianglesVector();

      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, restPoseVertices.data());

         glLineWidth(2.0);
         glColor4f(0.20f, 0.20f, 0.20f, 0.8f);
         glDrawElements(GL_TRIANGLES, restPoseTriangles.size(), GL_UNSIGNED_INT, restPoseTriangles.data());

         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      double radius = restPose.getBoundingBox().diagonal()/230.0;

      if(drawMode & DRAW_VERTICES){
         setSingleLighting();
         for(ulong i=0; i<restPose.getNumVertices(); ++i)
         {
            if (isVertexSelected[i])
               drawSphere(restPoseVertices[i*3+0], restPoseVertices[i*3+1], restPoseVertices[i*3+2], radius, 0.905, 0.305, 0.305);
            else
               drawSphere(restPoseVertices[i*3+0], restPoseVertices[i*3+1], restPoseVertices[i*3+2], radius, 0.99, 0.85, 0.39);
         }
      }
   }
}

void DrawableCage::drawWithNames()
{
   double radius = currentPose.getBoundingBox().diagonal()/200.0;
   const std::vector<double> & currentPoseVertices  = currentPose.getVerticesVector();

   if(drawMode & DRAW_VERTICES){
      for(ulong i=0; i<currentPose.getNumVertices(); ++i)
      {
         glPushMatrix();
         glPushName(vertex2PickableIndex[i]);
         drawSphere(currentPoseVertices[i*3+0], currentPoseVertices[i*3+1], currentPoseVertices[i*3+2], radius, 0.99, 0.85, 0.39);
         glPopName();
         glPopMatrix();
      }
   }
}

void DrawableCage::drawWithNamesRest()
{
   double radius = restPose.getBoundingBox().diagonal()/200.0;
   const std::vector<double> & restPoseVertices  = restPose.getVerticesVector();

   if(drawMode & DRAW_VERTICES){
      for(ulong i=0; i<restPose.getNumVertices(); ++i)
      {
         glPushMatrix();
         glPushName(vertex2PickableIndex[i]);
         drawSphere(restPoseVertices[i*3+0], restPoseVertices[i*3+1], restPoseVertices[i*3+2], radius, 0.99, 0.85, 0.39);
         glPopName();
         glPopMatrix();
      }
   }
}

bool DrawableCage::sceneCenter(cg3::Vec3d & center) const
{
   center = currentPose.getBoundingBox().center();
   return true;   //center is a valid value
}

bool DrawableCage::sceneRadius(double & radius) const
{
   radius = currentPose.getBoundingBox().diagonal();
   return true;   //center is a valid value
}

bool DrawableCage::getSelectedObjectsBarycenter(cg3::Point3d & barycenter, const bool restPose) const
{
   std::vector<cg3::Point3d> selectedVerticesPositions;

   for(ulong i=0; i<currentPose.getNumVertices(); ++i)
   {
      if(isVertexSelected[i])
      {
         if(restPose)
            selectedVerticesPositions.push_back(getRestPoseVertex(i));
         else
            selectedVerticesPositions.push_back(getCurrentPoseVertex(i));
      }
   }

   ulong numberOfSelectedVertices = selectedVerticesPositions.size();

   for(ulong i=0; i<numberOfSelectedVertices; ++i)
   {
      if(i==0)
         barycenter = selectedVerticesPositions[0];
      else
         barycenter += selectedVerticesPositions[i];
   }

   if(numberOfSelectedVertices)
   {
      barycenter /= static_cast<double>(numberOfSelectedVertices);
      return true;
   }

   return false;
}

void DrawableCage::selectObject(const ulong pickableIndex)
{
   ulong vertexIndex = pickableIndex2Vertex[pickableIndex];
   isVertexSelected[vertexIndex] = true;
   selectedVertices.insert(vertexIndex);
}

void DrawableCage::deselectObject(const ulong pickableIndex)
{
   ulong vertexIndex = pickableIndex2Vertex[pickableIndex];
   isVertexSelected[vertexIndex] = false;
   selectedVertices.erase(vertexIndex);
}

//Deformation
void DrawableCage::translate(const cg3::Vec3d & translation)
{
   if(activateTransformation)
   {
      for(unsigned long i=0; i<getNumVertices(); ++i)
      {
         if(isVertexSelected[i])
         {
            lastTranslations[3*i+0] = translation.x();
            lastTranslations[3*i+1] = translation.y();
            lastTranslations[3*i+2] = translation.z();
            cg3::Vec3d deformedPose = getCurrentPoseVertex(i);

            cg3::Vec3d newPose = deformedPose + translation;

            currentPose.setVertex(i,newPose);
            _refreshCharacterPose = true;
         }
         else
         {
            lastTranslations[3*i+0] = 0.0;
            lastTranslations[3*i+1] = 0.0;
            lastTranslations[3*i+2] = 0.0;
         }
      }
   }

}

void DrawableCage::rotate(const cg3::dQuaternion & rotation)
{

}

void DrawableCage::rotateRest(const cg3::dQuaternion & rotation)
{
   if(activateTransformation)
   {

      cg3::Point3d rotationCenter;
      getSelectedObjectsBarycenter(rotationCenter, true);

      for(ulong i=0; i<currentPose.getNumVertices(); ++i)
      {
         if(isVertexSelected[i])
         {
            cg3::Point3d pos = getRestPoseVertex(i);

            //from local to world coordinates
            pos -= rotationCenter;

            //rotate the position
            cg3::Point3d newPos = rotation.applyRotation(pos);

            //from world to local coordinates
            newPos += rotationCenter;

            setRestPoseVertex(i,newPos);
         }
         _refreshCharacterPose = true;
      }
   }
}

void DrawableCage::scale(const int delta)
{
   if(activateTransformation)
   {
      cg3::Point3d centerOfScaling;


      //computer center of scale
      if(getSelectedObjectsBarycenter(centerOfScaling))
      {

         for(unsigned long i=0; i<currentPose.getNumVertices(); ++i)
         {
            if(isVertexSelected[i])
            {
               cg3::Point3d pos = currentPose.getVertex(i);
               cg3::Vec3d dir = centerOfScaling - pos;
               dir *=0.05;

               if(delta<0)
               {
                  lastTranslations[3*i+0] = -dir.x();
                  lastTranslations[3*i+1] = -dir.y();
                  lastTranslations[3*i+2] = -dir.z();
                  currentPose.setVertex(i,pos-dir);
               }
               else
               {
                  lastTranslations[3*i+0] = dir.x();
                  lastTranslations[3*i+1] = dir.y();
                  lastTranslations[3*i+2] = dir.z();
                  currentPose.setVertex(i,pos+dir);
               }

               _refreshCharacterPose = true;
            }
            else
            {
               lastTranslations[3*i+0] = 0.0;
               lastTranslations[3*i+1] = 0.0;
               lastTranslations[3*i+2] = 0.0;
            }
         }
      }
   }
}

void DrawableCage::translateRest(const cg3::Vec3d &translation)
{
   if(activateTransformation)
   {
      for(unsigned long i=0; i<restPose.getNumVertices(); ++i)
      {
         if(isVertexSelected[i])
         {
            cg3::Vec3d restPose = getRestPoseVertex(i);
            cg3::Vec3d newRestPose = restPose + translation;
            setRestPoseVertex(i,newRestPose);
            _refreshCharacterPose = true;
         }
      }
   }
}

void DrawableCage::scaleRest(const int delta)
{
   if(activateTransformation)
   {
      cg3::Point3d centerOfScaling;


      //computer center of scale
      if(getSelectedObjectsBarycenter(centerOfScaling, true))
      {
         for(unsigned long i=0; i<restPose.getNumVertices(); ++i)
         {
            if(isVertexSelected[i])
            {
               cg3::Point3d pos = getRestPoseVertex(i);
               cg3::Vec3d dir = centerOfScaling - pos;
               dir *=0.05;

               if(delta<0)
               {
                  setRestPoseVertex(i,pos-dir);
               }
               else
               {
                  setRestPoseVertex(i,pos+dir);
               }

               _refreshCharacterPose = true;
            }
         }
      }
   }

}

void DrawableCage::activateCageRendering(bool activate)
{
   if (activate)  drawMode |=  DRAW_CAGE;
   else           drawMode &= ~DRAW_CAGE;
}
