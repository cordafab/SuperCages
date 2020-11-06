#include "drawableObject.h"

DrawableObject::DrawableObject()
{
   type = ABSTRACT;
}

DrawableObject::~DrawableObject()
{

}

bool DrawableObject::sceneCenter(cg3::Vec3d & center) const
{
   center.set(0.0, 0.0, 0.0);
   return false;
}

bool DrawableObject::sceneRadius(double & radius) const
{
   radius = 0.0;
   return false;
}

