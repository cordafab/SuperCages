#include "glUtils.h"

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

void drawCube(cg3::Vec3d center, double radius, float r, float g, float b, bool wireframe)
{
   std::vector<double> vertices
   {
      1, 1,-1,
      -1,-1,-1,
      -1, 1,-1,
      1, 1, 1,
      -1,-1, 1,
      1,-1, 1,
      1, 1,-1,
      1,-1, 1,
      1,-1,-1,
      1,-1,-1,
      -1,-1, 1,
      -1,-1,-1,
      -1,-1,-1,
      -1, 1, 1,
      -1, 1,-1,
      1, 1, 1,
      -1, 1,-1,
      -1, 1, 1,
      1, 1,-1,
      1,-1,-1,
      -1,-1,-1,
      1, 1, 1,
      -1, 1, 1,
      -1,-1, 1,
      1, 1,-1,
      1, 1, 1,
      1,-1, 1,
      1,-1,-1,
      1,-1, 1,
      -1,-1, 1,
      -1,-1,-1,
      -1,-1, 1,
      -1, 1, 1,
      1, 1, 1,
      1, 1,-1,
      -1, 1,-1
   };

   std::vector<int> tris
   {
      0, 1, 2,
      3, 4, 5,
      6, 7, 8,
      9,10,11,
      12,13,14,
      15,16,17,
      18,19,20,
      21,22,23,
      24,25,26,
      27,28,29,
      30,31,32,
      33,34,35
   };

   std::vector<double> normals
   {
      0, 0,-1,
      0, 0,-1,
      0, 0,-1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      0,-1, 0,
      0,-1, 0,
      0,-1, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,
      0, 0,-1,
      0, 0,-1,
      0, 0,-1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      0,-1, 0,
      0,-1, 0,
      0,-1, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      0, 1, 0,
      0, 1, 0,
      0, 1, 0
   };

   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);
      glLineWidth(1.0);
   }
   else
   {
      glEnable(GL_LIGHTING);
      glShadeModel(GL_SMOOTH);
   }
   glPushMatrix();
   glTranslated(center.x(), center.y(), center.z());
   glScaled(radius, radius, radius);
   glColor3f(r,g,b);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
   glNormalPointer(GL_DOUBLE, 0, normals.data());
   glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   glPopMatrix();
   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_LIGHTING);
   }
}

void drawDiamond(cg3::Vec3d center, double radius, float r, float g, float b, bool wireframe)
{
   std::vector<double> vertices
   {
      0, 1, 0,
      0, 0, 1,
      1, 0, 0,

      0, 1, 0,
      1, 0, 0,
      0, 0,-1,

      0, 1, 0,
      0, 0,-1,
      -1, 0, 0,

      0, 1, 0,
      -1, 0, 0,
      0, 0, 1,

      0,-1, 0,
      0, 0, 1,
      -1, 0, 0,

      0,-1, 0,
      1, 0, 0,
      0, 0, 1,

      0,-1, 0,
      0, 0,-1,
      1, 0, 0,

      0,-1, 0,
      -1, 0, 0,
      0, 0,-1
   };

   std::vector<int> tris
   {
      0, 1, 2,
      3, 4, 5,
      6, 7, 8,
      9,10,11,
      12,13,14,
      15,16,17,
      18,19,20,
      21,22,23,
   };

   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);
      glLineWidth(4.0);
   }
   else
   {
      glEnable(GL_LIGHTING);
      glShadeModel(GL_SMOOTH);
   }
   glPushMatrix();
   glTranslated(center.x(), center.y(), center.z());
   glScaled(radius, radius, radius);
   glColor3f(r,g,b);
   glEnableClientState(GL_VERTEX_ARRAY);
   //glEnableClientState(GL_NORMAL_ARRAY);
   glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
   //glNormalPointer(GL_DOUBLE, 0, normals.data());
   glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
   //glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
   glPopMatrix();
   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_LIGHTING);
   }
}

void drawSphere(cg3::Vec3d center, double radius, float r, float g, float b, bool wireframe)
{
   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_LIGHTING);
      glShadeModel(GL_FLAT);
      glLineWidth(1.0);
   }
   else
   {
      glEnable(GL_LIGHTING);
      glShadeModel(GL_SMOOTH);
   }

   glPushMatrix();
   glTranslated(center.x(), center.y(), center.z());
   glColor3f(r, g, b);
   GLUquadric *sphere = gluNewQuadric();
   gluQuadricOrientation(sphere,GLU_OUTSIDE);
   gluSphere(sphere, radius, 20, 20);
   gluDeleteQuadric(sphere);
   glPopMatrix();
   if(wireframe)
   {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glEnable(GL_LIGHTING);
   }
}

void drawRay(cg3::dRay ray)
{
   cg3::Point3d origin;
   cg3::Point3d pointOnRay;

   origin = ray.origin();
   pointOnRay = ray.getPointOnRay(250000.0);

   glBegin(GL_LINES);
   glColor3f(1.0f, 0.0f, 0.0f);
   glVertex3d(origin.x(), origin.y(), origin.z());
   glVertex3d(pointOnRay.x(), pointOnRay.y(), pointOnRay.z());
   glEnd();
}

void drawPlane(cg3::dPlane plane)
{
   cg3::Point3d origin;
   cg3::Point3d pointOnRay;

   //origin = ray.origin();
   //pointOnRay = ray.getPointOnRay(250000.0);

   /*glBegin(GL_LINES);
   glColor3f(1.0f, 0.0f, 0.0f);
   glVertex3d(origin.x(), origin.y(), origin.z());
   glVertex3d(pointOnRay.x(), pointOnRay.y(), pointOnRay.z());
   glEnd();*/
}

void drawSelectionRectangle(int top, int bottom, int left, int right)
{
   //This function operates in screen coordinates space!
   glDisable(GL_LIGHTING);
   //glEnable(GL_BLEND);

   glColor4f(0.0, 0.0, 0.3f, 0.2f);
   glBegin(GL_QUADS);
   glVertex2i(left,  top);
   glVertex2i(right, top);
   glVertex2i(right, bottom);
   glVertex2i(left, bottom);
   glEnd();

   //glDisable(GL_BLEND);

   glLineWidth(2.0);
   glColor4f(0.2f, 0.2f, 0.2f, 0.7f);
   glBegin(GL_LINE_LOOP);
   glVertex2i(left, top);
   glVertex2i(right, top);
   glVertex2i(right, bottom);
   glVertex2i(left, bottom);
   glEnd();

   glEnable(GL_LIGHTING);
}

void initLighting () {
#ifdef CUSTOM_LIGHTS
   GLfloat light_position0[4] = {0, 0, 50, 0};
   GLfloat light_position1[4] = {52, 16, 50, 0};
   GLfloat light_position2[4] = {26, 48, 50, 0};
   GLfloat light_position3[4] = {-16, 52, 50, 0};
   GLfloat light_position4[4] = {0, 0, -50, 0};
   GLfloat direction1[3] = {-52,-16,-50};
   GLfloat direction2[3] = {-26,-48,-50};
   GLfloat direction3[3] = {16,-52,-50};
   GLfloat direction4[3] = {52,16,50};
   GLfloat color1[4] = {1,0, 0, 1};
   GLfloat color2[4] = {0, 1, 0, 1};
   GLfloat color3[4] = {0, 0, 1, 1};
   GLfloat color4[4] = {1, 1, 1, 0.5f};
   GLfloat ambient[4] = {0.1f, 0.1f, 0.1f, 0.5f};
   glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
   glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
   glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
   glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
   glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
   glLightfv (GL_LIGHT2, GL_POSITION, light_position2);
   glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, direction2);
   glLightfv (GL_LIGHT2, GL_DIFFUSE, color2);
   glLightfv (GL_LIGHT2, GL_SPECULAR, color2);
   glLightfv (GL_LIGHT3, GL_POSITION, light_position3);
   glLightfv (GL_LIGHT3, GL_SPOT_DIRECTION, direction3);
   glLightfv (GL_LIGHT3, GL_DIFFUSE, color3);
   glLightfv (GL_LIGHT4, GL_POSITION, light_position4);
   glLightfv (GL_LIGHT4, GL_SPOT_DIRECTION, direction4);
   glLightfv (GL_LIGHT4, GL_DIFFUSE, color4);
   glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
#endif
}

void initMaterial () {
#ifdef CUSTOM_LIGHTS
   GLfloat material_color[4] = {1,1,1,1.0f};
   GLfloat material_specular[4] = {0.85,0.85,0.85,1.0};
   GLfloat material_ambient[4] = {0.0,0.0,0.0,0.0};
   glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
   glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
   glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
   glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 128.0f);
#endif
}

void initInverseMaterial () {
#ifdef CUSTOM_LIGHTS
   GLfloat material_color[4] = {0.0, 0.0, 0.0, 1.0f};
   GLfloat material_specular[4] = {0.0,0.0,0.0,1.0};
   GLfloat material_ambient[4] = {0.0,0.0,0.0,0.0};
   glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
   glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
   glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
   glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 0);
#endif
}

void setSingleLighting () {
#ifdef CUSTOM_LIGHTS
   glEnable (GL_LIGHT0);
   glDisable (GL_LIGHT1);
   glDisable (GL_LIGHT2);
   glDisable (GL_LIGHT3);
   glEnable (GL_LIGHT4);
#endif
}

void setMultiLighting () {
#ifdef CUSTOM_LIGHTS
   glDisable (GL_LIGHT0);
   glEnable (GL_LIGHT1);
   glEnable (GL_LIGHT2);
   glEnable (GL_LIGHT3);
   glDisable (GL_LIGHT4);
#endif
}

void disableLighting ()
{
#ifdef CUSTOM_LIGHTS
   glDisable (GL_LIGHT0);
   glDisable (GL_LIGHT1);
   glDisable (GL_LIGHT2);
   glDisable (GL_LIGHT3);
   glDisable (GL_LIGHT4);
#endif
}
