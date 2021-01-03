#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include "geom/vec3.h"
#include "geom/quaternion.h"

#ifdef EIGEN_AVAILABLE
#include "Eigen/Geometry"
#endif

namespace cg3 {


//TO DO: Delete Eigen dependency [check personal OneNote notebook]
typedef Eigen::Transform<double, 3, Eigen::Affine> eigenTransformation;

class Transform
{
private:
   //https://eigen.tuxfamily.org/dox/group__TutorialGeometry.html
   cg3::eigenTransformation transformation;

public:
   Transform();
   Transform(double tx, double ty, double tz);
   Transform(double rx, double ry, double rz, double tx, double ty, double tz);
   Transform(cg3::dQuaternion r);
   Transform(double mat[16]);
   Transform(cg3::Vec3d col1, cg3::Vec3d col2, cg3::Vec3d col3);
   Transform(cg3::eigenTransformation t); //This is ugly

   cg3::Vec3d getTranslation() const;
   cg3::dQuaternion getRotation() const;

   void setToZero();

   void setTranslation(const cg3::Vec3d & t);

   Transform cumulateWith(const Transform & t) const;
   Transform inverse() const;

   cg3::Vec3d applyToPoint(cg3::Vec3d p) const;
   cg3::Vec3d applyToPoint(double x, double y, double z) const;

   Transform transformTowardsOrigin(Transform t);

   void data(std::vector<double> & v) const;

   inline double & operator() (int x, int y)       { return transformation(x,y); }
   inline double   operator() (int x, int y) const { return transformation(x,y); }

   inline const cg3::eigenTransformation & getEigenTransformation() const { return transformation; }
   inline       cg3::eigenTransformation & getEigenTransformation()       { return transformation; }

   Transform operator+(const Transform & other) const;
   Transform operator*(const Transform & other) const;
   Transform operator*(const double s) const;

   Transform interpolate(const Transform & other, double a) const;
};

std::ostream & operator<< (std::ostream & stream, const Transform & t);

}

#endif // TRANSFORM_H
