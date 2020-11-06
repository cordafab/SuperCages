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
                          const std::vector<int>    & tris)
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
   isVertexSelected.resize(getNumVertices(), false);
   lastTranslations.resize(getNumVertices()*3,0.0);

   pickerController = PickerController::get();

   //assign PickableIndexes to every vertex
   for(int i=0; i<getNumVertices(); ++i)
   {
      int pickableIndex = pickerController->generateIndex(this);
      pickableIndex2Vertex[pickableIndex] = i;
      vertex2PickableIndex[i] = pickableIndex;
   }
}

void DrawableCage::clear()
{
    Cage::clear();

    for(int i=0; i<getNumVertices(); ++i)
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
    lastTranslations.clear();
}

void DrawableCage::draw() const
{
   if (drawMode & DRAW_CAGE)
   {
      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, vertices.data());

         glLineWidth(7.0);
         //glColor4f(0.99, 0.85, 0.39, 0.7);
         glColor4f(0.20f, 0.20f, 0.20f, 0.8f);
         //glLineWidth(0.2);
         //glColor4f(0,0,0,0.1);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());

         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      double radius = boundingBox.diagonal()/400.0;
      //double radius = boundingBox.diagonal()/150.0;

      if(drawMode & DRAW_VERTICES)
      {
         setSingleLighting();
         for(int i=0; i<getNumVertices(); ++i)
         {
            if (isVertexSelected[i])
               drawSphere(getVertex(i), radius, 0.905, 0.305, 0.305);
            else
               //drawSphere(getVertex(i), radius, 0.5, 0.5, 0.5);
               drawSphere(getVertex(i), radius, 0.99, 0.85, 0.39);
         }
      }

      //create a drawFrame fuction in glUtils
      if (drawMode & DRAW_FRAMES)
      {
         /*double length = boundingBox.diagonal()/50.0;
         for(int i=0; i<getNumVertices(); ++i)
         {
            cg3::Frame f = deformedPoseFrames[i];
            cg3::Vec3d p = f.o();
            cg3::Vec3d s;

            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            glBegin(GL_LINES);
            s = p + (f.u()*length);
            glColor3f(1.0,0.0,0.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            s = p + (f.v()*length);
            glColor3f(0.0,1.0,0.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            s = p + (f.w()*length);
            glColor3f(0.0,0.0,1.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            glEnd();
         }*/
      }
   }
}

void DrawableCage::drawRest() const
{
   if (drawMode & DRAW_CAGE)
   {
      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, restPoseVertices.data());

         //glLineWidth(3.0);
         //glColor3f(0.99, 0.85, 0.39);
         glLineWidth(5.0);
         //glColor4f(0.99, 0.85, 0.39, 0.7);
         glColor4f(0.20f, 0.20f, 0.20f, 0.7f);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());

         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }

      double radius = boundingBox.diagonal()/230.0;
      //double radius = boundingBox.diagonal()/150.0;

      if(drawMode & DRAW_VERTICES){
         setSingleLighting();
         for(int i=0; i<getNumVertices(); ++i)
         {
            if (isVertexSelected[i])
               drawSphere(getRestPoseVertex(i), radius, 0.905, 0.305, 0.305);
            else
               //drawSphere(getVertex(i), radius, 0.5, 0.5, 0.5);
               drawSphere(getRestPoseVertex(i), radius, 0.99, 0.85, 0.39);
         }
      }

      //create a drawFrame fuction in glUtils
      if (drawMode & DRAW_FRAMES)
      {
         /*double length = boundingBox.diagonal()/50.0;
         for(int i=0; i<getNumVertices(); ++i)
         {
            cg3::Frame f = restPoseFrames[i];
            cg3::Vec3d p = f.o();
            cg3::Vec3d s;

            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            glBegin(GL_LINES);
            s = p + (f.u()*length);
            glColor3f(1.0,0.0,0.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            s = p + (f.v()*length);
            glColor3f(0.0,1.0,0.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            s = p + (f.w()*length);
            glColor3f(0.0,0.0,1.0);
            glVertex3f(p.x(), p.y(), p.z());
            glVertex3f(s.x(), s.y(), s.z());
            glEnd();
         }*/
      }
   }
}

void DrawableCage::drawWithNames()
{
   double radius = boundingBox.diagonal()/200.0;

   if(drawMode & DRAW_VERTICES){
      for(int i=0; i<getNumVertices(); ++i)
      {
         glPushMatrix();
         glPushName(vertex2PickableIndex[i]);
         drawSphere(getVertex(i), radius, 0.0, 0.0, 1.0);
         glPopName();
         glPopMatrix();
      }
   }
}

void DrawableCage::drawWithNamesRest()
{
   double radius = boundingBox.diagonal()/200.0;

   if(drawMode & DRAW_VERTICES){
      for(int i=0; i<getNumVertices(); ++i)
      {
         glPushMatrix();
         glPushName(vertex2PickableIndex[i]);
         drawSphere(getRestPoseVertex(i), radius, 0.0, 0.0, 1.0);
         glPopName();
         glPopMatrix();
      }
   }
}

bool DrawableCage::sceneCenter(cg3::Vec3d & center) const
{
   center = boundingBox.center();
   return true;   //center is a valid value
}

bool DrawableCage::sceneRadius(double & radius) const
{
   radius = boundingBox.diagonal();
   return true;   //center is a valid value
}

bool DrawableCage::getSelectedObjectsBarycenter(cg3::Point3d & barycenter, const bool restPose) const
{
   std::vector<cg3::Point3d> selectedVerticesPositions;

   for(int i=0; i<getNumVertices(); ++i)
   {
      if(isVertexSelected[i])
      {
         if(restPose)
            selectedVerticesPositions.push_back(getRestPoseVertex(i));
         else
            selectedVerticesPositions.push_back(getVertex(i));
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
            cg3::Vec3d deformedPose = getVertex(i);

            cg3::Vec3d newPose = deformedPose + translation;

            setVertex(i,newPose);
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

      for(ulong i=0; i<getNumVertices(); ++i)
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

         for(unsigned long i=0; i<getNumVertices(); ++i)
         {
            if(isVertexSelected[i])
            {
               cg3::Point3d pos = getVertex(i);
               cg3::Vec3d dir = centerOfScaling - pos;
               dir *=0.05;

               if(delta<0)
               {
                  lastTranslations[3*i+0] = -dir.x();
                  lastTranslations[3*i+1] = -dir.y();
                  lastTranslations[3*i+2] = -dir.z();
                  setVertex(i,pos-dir);
               }
               else
               {
                  lastTranslations[3*i+0] = dir.x();
                  lastTranslations[3*i+1] = dir.y();
                  lastTranslations[3*i+2] = dir.z();
                  setVertex(i,pos+dir);
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
      for(unsigned long i=0; i<getNumVertices(); ++i)
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
         for(unsigned long i=0; i<getNumVertices(); ++i)
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
