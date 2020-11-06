#include "drawableCharacter.h"

#ifdef __APPLE__
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "common/texture.h"
#include "GUI/glUtils.h"

DrawableCharacter::DrawableCharacter()
   : Character()
{
   init();
}

DrawableCharacter::DrawableCharacter(const std::vector<double> & vertices  ,
                                     const std::vector<int>    & tris      )
{
    create(vertices, tris);
}

DrawableCharacter::DrawableCharacter(const std::vector<double> & vertices,
                                     const std::vector<int>    & tris,
                                     const std::vector<double> & vCut,
                                     const std::vector<float>  & uv_,
                                     const std::vector<int>    & tCut)
{
    create(vertices,
           tris,
           vCut,
           uv_,
           tCut);
}

bool DrawableCharacter::create(const std::vector<double>    & vertices,
                               const std::vector<int>       & tris)
{
    clear();

    Character::create(vertices,tris);

    init();

    return true;
}

bool DrawableCharacter::create(const std::vector<double>    & vertices,
                               const std::vector<int>       & tris,
                               const std::vector<double>    & vCut,
                               const std::vector<float>     & uv_,
                               const std::vector<int>       & tCut)
{
    clear();
    //TODO: Try to solve the triplicate "Trimesh::clear()"
    //and duplicate "Character::clear() call

    Character::create(vertices,tris);

    init();

    verticesCut = vCut;
    uv          = uv_;
    trisCut     = tCut;

    normalsCut.resize(verticesCut.size());
    originalToCut.resize(verticesCut.size()/3);

    linkOriginalVerticesToCut();
    updateTexture1D(texture1D);

    return true;
}

DrawableCharacter::~DrawableCharacter()
{

}

void DrawableCharacter::init()
{
   type           = TRIMESH;
   drawMode       = DRAW_TRIMESH | DRAW_SMOOTH;

   textureId1D = 0;
}

void DrawableCharacter::clear()
{
    Character::clear();

    verticesCut.clear();
    uv.clear();
    trisCut.clear();

    normalsCut.clear();
    originalToCut.clear();

    drawMode = 0;
    textureId1D = 0;
    texture1DCoords.clear();

    //TODO: clear texture2D
}

void DrawableCharacter::draw() const
{
   if (drawMode & DRAW_TRIMESH)
   {

      if (drawMode & DRAW_SMOOTH)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);
         setMultiLighting();
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, verticesNorm.data());
         glColor3f(0.9,0.9,0.9);
         //glColor3f(0.53,0.60,0.85);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_VERTEX_ARRAY);
         //setSingleLighting();
      } else
      if (drawMode & DRAW_FLAT)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setMultiLighting();
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, verticesNorm.data());
         glColor3f(1.0,1.0,1.0);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_VERTEX_ARRAY);
      } else
      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(0.1);
         glDepthRange(0.0, 1.0);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
         glColor4f(0.3,0.3,0.3,0.8);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else
      if (drawMode & DRAW_TEXTURE1D)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);
         setSingleLighting();
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, vertices.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, verticesNorm.data());
         glEnable(GL_TEXTURE_1D);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(1, GL_FLOAT, 0, texture1DCoords.data());
         glColor3f(1.0,1.0,1.0);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
         glDisable(GL_TEXTURE_1D);
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_VERTEX_ARRAY);
      } else
      if (drawMode & DRAW_TEXTURE2D)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);
         setSingleLighting();

         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(2, GL_FLOAT, 0, uv.data());
         glColor3f(1,1,1);
         glDisable(GL_COLOR_MATERIAL);
         glEnable(GL_TEXTURE_2D);

         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, verticesCut.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, normalsCut.data());
         glDrawElements(GL_TRIANGLES, trisCut.size(), GL_UNSIGNED_INT, trisCut.data());
         glDisableClientState(GL_VERTEX_ARRAY);
         glDisableClientState(GL_NORMAL_ARRAY);

         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
         glDisable(GL_TEXTURE_2D);
         glEnable(GL_COLOR_MATERIAL);
      }
   }
}

void DrawableCharacter::drawRest() const
{
   if (drawMode & DRAW_TRIMESH)
   {
      if ((drawMode & DRAW_SMOOTH) || (drawMode & DRAW_TEXTURE2D) || (drawMode & DRAW_TEXTURE1D))
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_SMOOTH);
         setSingleLighting();
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, restPoseVertices.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, restPoseVerticesNormals.data());
         glColor3f(0.9,0.9,0.9);
         //glColor3f(0.53,0.60,0.85);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_VERTEX_ARRAY);
      } else
      if (drawMode & DRAW_FLAT)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, restPoseVertices.data());
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_DOUBLE, 0, restPoseVerticesNormals.data());
         glColor3f(1.0,1.0,1.0);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_VERTEX_ARRAY);
      } else
      if (drawMode & DRAW_WIREFRAME)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(0.1);
         glDepthRange(0.0, 1.0);
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(3, GL_DOUBLE, 0, restPoseVertices.data());
         glColor4f(0.3,0.3,0.3,0.8);
         glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, tris.data());
         glDisableClientState(GL_VERTEX_ARRAY);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
   }
}

bool DrawableCharacter::sceneCenter(cg3::Vec3d & center) const
{
   center = boundingBox.center();
   return true;
}

bool DrawableCharacter::sceneRadius(double & radius) const
{
   radius = boundingBox.diagonal();
   return true;
}

void DrawableCharacter::activateCharacterRendering(bool activate)
{
   if (activate)  drawMode |=  DRAW_TRIMESH;
   else           drawMode &= ~DRAW_TRIMESH;
}

void DrawableCharacter::activateSmoothColouration()
{
   drawMode &= ~DRAW_FLAT;
   drawMode |=  DRAW_SMOOTH;
   drawMode &= ~DRAW_WIREFRAME;
   drawMode &= ~DRAW_TEXTURE1D;
   drawMode &= ~DRAW_TEXTURE2D;
}

void DrawableCharacter::activateFlatColouration()
{
   drawMode |=  DRAW_FLAT;
   drawMode &= ~DRAW_SMOOTH;
   drawMode &= ~DRAW_WIREFRAME;
   drawMode &= ~DRAW_TEXTURE1D;
   drawMode &= ~DRAW_TEXTURE2D;
}

void DrawableCharacter::activateWireframe()
{
   drawMode &= ~DRAW_FLAT;
   drawMode &= ~DRAW_SMOOTH;
   drawMode |=  DRAW_WIREFRAME;
   drawMode &= ~DRAW_TEXTURE1D;
   drawMode &= ~DRAW_TEXTURE2D;
}

void DrawableCharacter::activateTexture1D()
{
   drawMode &= ~DRAW_FLAT;
   drawMode &= ~DRAW_SMOOTH;
   drawMode &= ~DRAW_WIREFRAME;
   drawMode |=  DRAW_TEXTURE1D;
   drawMode &= ~DRAW_TEXTURE2D;
}

void DrawableCharacter::activateTexture2D()
{
   drawMode &= ~DRAW_FLAT;
   drawMode &= ~DRAW_SMOOTH;
   drawMode &= ~DRAW_WIREFRAME;
   drawMode &= ~DRAW_TEXTURE1D;
   drawMode |=  DRAW_TEXTURE2D;
}

void DrawableCharacter::activateSkelCageBlending(bool activate)
{
   if (activate)  drawMode |=  DRAW_SKELCAGEBLEND;
   else           drawMode &= ~DRAW_SKELCAGEBLEND;
}

void DrawableCharacter::updateCutVerticesPosition()
{
   if (drawMode & DRAW_SKELCAGEBLEND)
   {
      for(unsigned long i=0; i < verticesCut.size()/3; ++i)
      {
         verticesCut[i*3+0] = skelCageBlendVertices[originalToCut[i]*3+0];
         verticesCut[i*3+1] = skelCageBlendVertices[originalToCut[i]*3+1];
         verticesCut[i*3+2] = skelCageBlendVertices[originalToCut[i]*3+2];

         normalsCut[i*3+0] = verticesNorm[originalToCut[i]*3+0];
         normalsCut[i*3+1] = verticesNorm[originalToCut[i]*3+1];
         normalsCut[i*3+2] = verticesNorm[originalToCut[i]*3+2];
      }
   }
   else
   {
      for(unsigned long i=0; i < verticesCut.size()/3; ++i)
      {
         verticesCut[i*3+0] = vertices[originalToCut[i]*3+0];
         verticesCut[i*3+1] = vertices[originalToCut[i]*3+1];
         verticesCut[i*3+2] = vertices[originalToCut[i]*3+2];

         normalsCut[i*3+0] = verticesNorm[originalToCut[i]*3+0];
         normalsCut[i*3+1] = verticesNorm[originalToCut[i]*3+1];
         normalsCut[i*3+2] = verticesNorm[originalToCut[i]*3+2];
      }
   }
}

void DrawableCharacter::updateTexture1DCoords(std::vector<float> _textureCoords)
{
   texture1DCoords = _textureCoords;
}

void DrawableCharacter::updateTexture1D(const GLubyte tex[])
{
   if (textureId1D > 0)
   {
      glDeleteTextures(1, &textureId1D);
   }

   glGenTextures(1, &textureId1D);
   glBindTexture(GL_TEXTURE_1D, textureId1D);

   glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_R,     GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_BORDER);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void DrawableCharacter::updateTexture2D(const double texUnitScalar, const char *bitmap)
{
   texture2D.scaling_factor = texUnitScalar;
   cinolib::texture_bitmap(texture2D, bitmap);

   glBindTexture(GL_TEXTURE_2D, texture2D.id);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture2D.size, texture2D.size, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture2D.data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glGenerateMipmap(GL_TEXTURE_2D);
}

void DrawableCharacter::linkOriginalVerticesToCut()
{
   //VERY INEFFICIENT!!
   for (unsigned long i = 0; i < verticesCut.size()/3; ++i) {
      for (unsigned long j = 0; j < vertices.size()/3; ++j) {
         if((verticesCut[i*3+0] == vertices[j*3+0]) &&
            (verticesCut[i*3+1] == vertices[j*3+1]) &&
            (verticesCut[i*3+2] == vertices[j*3+2])  )
         {
            originalToCut[i] = j;
            normalsCut[i*3+0] = verticesNorm[j*3+0];
            normalsCut[i*3+1] = verticesNorm[j*3+1];
            normalsCut[i*3+2] = verticesNorm[j*3+2];
            break;
         }
      }
   }
}
