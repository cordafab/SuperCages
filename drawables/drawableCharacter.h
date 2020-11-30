#ifndef DRAWABLECHARACTER_H
#define DRAWABLECHARACTER_H

#include "drawables/drawableObject.h"
#include "rigs/character.h"

#define CINOLIB_USES_OPENGL
#define CINOLIB_USES_QT

#include "cinolib/textures/textures.h"


class DrawableCharacter
      : public Character,
        public DrawableObject
{
public:

   enum
   {
      DRAW_TRIMESH          = 0x00000001,
      DRAW_VERTICES         = 0x00000002,
      DRAW_FLAT             = 0x00000004,
      DRAW_SMOOTH           = 0x00000008,
      DRAW_WIREFRAME        = 0x00000010,
      DRAW_TEXTURE1D        = 0x00000020,
      DRAW_TEXTURE2D        = 0x00000040
   };

   DrawableCharacter();
   DrawableCharacter(const std::vector<double>      & vertices ,
                     const std::vector<int>         & tris   );
   DrawableCharacter(const std::vector<double>      & vertices,
                     const std::vector<int>         & tris,
                     const std::vector<double>      & vCut,
                     const std::vector<float>       & uv_,
                     const std::vector<int>         & tCut);

   bool create      (const std::vector<double>      & vertices ,
                     const std::vector<int>         & tris   );
   bool create      (const std::vector<double>      & vertices,
                     const std::vector<int>         & tris,
                     const std::vector<double>      & vCut,
                     const std::vector<float>       & uv_,
                     const std::vector<int>         & tCut);

   ~DrawableCharacter();

   void init();
   void clear();

   //DrawableObject interface
   void draw()                             const;
   void drawRest()                         const;
   bool sceneCenter(cg3::Vec3d & center)   const;
   bool sceneRadius(double & radius)       const;

   //Rendering settings
   void activateCharacterRendering(bool activate);
   void activateSmoothColouration();
   void activateFlatColouration();
   void activateWireframe();
   void activateTexture1D();
   void activateTexture2D();
   void updateCutVerticesPosition();
   void updateTexture1DCoords(std::vector<float> _textureCoords);
   void updateTexture1D(const GLubyte tex[]);
   void updateTexture2D(const double texUnitScalar, const char *bitmap);

   inline const std::vector<float> & getUvVector() const {return uv;}

   std::string diffuse_tex; //To incupsulate

protected:

   std::vector<double> verticesCut;
   std::vector<double> normalsCut;
   std::vector<float>  uv;
   std::vector<int>    trisCut;

   void linkOriginalVerticesToCut();
   std::vector<ulong> originalToCut;

   int drawMode;

   unsigned int textureId1D;
   std::vector<float> texture1DCoords;

   cinolib::Texture texture2D;

};

#endif // DRAWABLECHARACTER_H
