#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "geom/vec3.h"

class BoundingBox
{
public:

   BoundingBox()
   {
      clear();
   }

   cg3::Vec3d min, max;

   inline void clear()
   {
      min = cg3::Vec3d( FLT_MAX,   FLT_MAX,   FLT_MAX);
      max = cg3::Vec3d(-FLT_MAX,  -FLT_MAX,  -FLT_MAX);
   }

   inline cg3::Vec3d center()   const { return (min+max)*0.5; }
   inline double     diagonal() const { return (min-max).length(); }

};

#endif // BOUNDINGBOX_H
