#ifndef DRAWABLECAGE_H
#define DRAWABLECAGE_H

#include "drawables/drawableObject.h"
#include "drawables/pickableObject.h"
#include "drawables/pickerController.h"
#include "rigs/cage.h"

#include <unordered_set>

class DrawableCage
      : public Cage,
        public DrawableObject,
        public PickableObject
{
public:

   enum
   {
      DRAW_CAGE         = 0x00000001,
      DRAW_VERTICES     = 0x00000002,
      DRAW_WIREFRAME    = 0x00000004
   };

   DrawableCage();
   DrawableCage(const std::vector<double> & vertices ,
                const std::vector<int>    & tris     );

   bool create  (const std::vector<double> & vertices ,
                 const std::vector<int >   & tris     );

   ~DrawableCage();

   void init();
   void clear();

   //DrawableObject interface
   void draw() const;
   void drawRest() const;
   void drawWithNames();
   void drawWithNamesRest();
   bool sceneCenter(cg3::Vec3d & center) const;
   bool sceneRadius(double & radius) const;

   //skinning flags
   inline bool refreshCharacterPose() const { return _refreshCharacterPose; }
   inline void characterPoseRefreshed() { _refreshCharacterPose = false; }

   //PickableObject interface
   bool getSelectedObjectsBarycenter(cg3::Point3d & barycenter, const bool restPose = false) const;
   void selectObject(const ulong pickableIndex);
   void deselectObject(const ulong pickableIndex);

   //Deformation
   void translate (const cg3::Vec3d        & translation);
   void rotate    (const cg3::dQuaternion  & rotation);
   void scale     (const int                 delta);

   //Deformation Rest Pose
   void translateRest (const cg3::Vec3d        & translation);
   void rotateRest    (const cg3::dQuaternion  & rotation);
   void scaleRest     (const int                 delta);

   inline const std::vector<bool> & getSelectedVertices() const
      { return isVertexSelected; }

   void activateCageRendering(bool activate);

protected:

   int drawMode;
   bool _refreshCharacterPose;

   //technical stuff for picking
   PickerController*          pickerController;
   std::map<ulong,ulong>      pickableIndex2Vertex;
   std::map<ulong,ulong>      vertex2PickableIndex;
   std::vector<bool>          isVertexSelected;
   std::unordered_set<ulong>  selectedVertices;
};


#endif // DRAWABLECAGE_H
