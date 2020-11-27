#include "transform.h"
#include <vector>

namespace cg3 {

Transform::Transform()
{
   transformation.setIdentity();
}

Transform::Transform(double tx, double ty, double tz)
{
   Eigen::Translation3d translation(tx, ty, tz);

   transformation = translation;
}

//Euler angles+translation
Transform::Transform(double rx, double ry, double rz, double tx, double ty, double tz)
{

   //TO DO: Optimize this
   constexpr double gradToRad = M_PI / 180;

   cg3::Transform T_rx (cg3::dQuaternion(cg3::Vec3d(1.0,0.0,0.0),rx*gradToRad));
   cg3::Transform T_ry (cg3::dQuaternion(cg3::Vec3d(0.0,1.0,0.0),ry*gradToRad));
   cg3::Transform T_rz (cg3::dQuaternion(cg3::Vec3d(0.0,0.0,1.0),rz*gradToRad));

   //cg3::Transform T_rzy = T_rz.cumulateWith(T_ry);
   //cg3::Transform T_rzyx = T_rzy.cumulateWith(T_rx);

   cg3::Transform T_rzyx = T_rz * T_ry * T_rx;

   cg3::Transform t (tx, ty, tz);

   transformation=(t.cumulateWith(T_rzyx)).transformation;
}

Transform::Transform(dQuaternion r)
{
   Eigen::Quaterniond rotation(r.s(), r.x(), r.y(), r.z());

   transformation = rotation;
}

Transform::Transform(dQuaternion r, cg3::Vec3d t)
{
   /*Eigen::Quaterniond rotation(r.s(), r.x(), r.y(), r.z());
   Eigen::Vector3d translation(t.x(), t.y(), t.z());

   transformation = rotation;
   transformation.translation() = translation;*/
}

Transform::Transform(double mat[16])
{
   Eigen::Matrix4d m; //It is column major

   m.col(0) = Eigen::Vector4d(mat[0]  , mat[1]  , mat[2]  , mat[3] );
   m.col(1) = Eigen::Vector4d(mat[4]  , mat[5]  , mat[6]  , mat[7] );
   m.col(2) = Eigen::Vector4d(mat[8]  , mat[9]  , mat[10] , mat[11]);
   m.col(3) = Eigen::Vector4d(mat[12] , mat[13] , mat[14] , mat[15]);

   transformation = m;
}

Transform::Transform(Vec3d col1, Vec3d col2, Vec3d col3)
{
   Eigen::Matrix4d m; //It is column major

   m.col(0) = Eigen::Vector4d(col1.x(), col1.y(), col1.z(), 0 );
   m.col(1) = Eigen::Vector4d(col2.x(), col2.y(), col2.z(), 0 );
   m.col(2) = Eigen::Vector4d(col3.x(), col3.y(), col3.z(), 0 );
   m.col(3) = Eigen::Vector4d(0 , 0 , 0 , 1 );

   transformation = m;
}

Transform::Transform(cg3::eigenTransformation t)
{
   transformation = t;
}

cg3::Vec3d Transform::getTranslation() const
{
   Eigen::Vector3d vecT = transformation.translation();
   return cg3::Vec3d(vecT[0], vecT[1], vecT[2]);
}

dQuaternion Transform::getRotation() const
{
   //mi basta passargli la matrice intera e elimina la traslazione?
   Eigen::Quaterniond q(transformation.rotation());
   return cg3::dQuaternion (q.x(),q.y(),q.z(),q.w());
}

void Transform::setToZero()
{
   Eigen::Matrix4d m;
   m.setZero();
   transformation = m;
}

void Transform::setTranslation(const cg3::Vec3d & t)
{
   transformation.translation() = Eigen::Vector3d(t.x(), t.y(), t.z());
}

Transform Transform::cumulateWith(const Transform & t) const
{
   return (*this) * t;
}

Transform Transform::inverse() const
{
   return Transform(transformation.inverse(Eigen::Affine));
}

cg3::Vec3d Transform::applyToPoint(cg3::Vec3d p) const
{
   Eigen::Vector3d _p(p.x(), p.y(), p.z());

   Eigen::Vector3d p1 = transformation * _p;
   return cg3::Vec3d(p1[0], p1[1], p1[2]);
}

Vec3d Transform::applyToPoint(double x, double y, double z) const
{
   Eigen::Vector3d _p(x, y, z);

   Eigen::Vector3d p1 = transformation * _p;
   return cg3::Vec3d(p1[0], p1[1], p1[2]);
}

Transform Transform::transformTowardsOrigin(Transform t)
{
   Transform thisTransform(transformation);
   Eigen::Vector3d vecT = transformation.translation();
   thisTransform.transformation(0,3) = 0;
   thisTransform.transformation(1,3) = 0;
   thisTransform.transformation(2,3) = 0;
   thisTransform.transformation = t.transformation * thisTransform.transformation;
   thisTransform.transformation(0,3) += vecT[0];
   thisTransform.transformation(1,3) += vecT[1];
   thisTransform.transformation(2,3) += vecT[2];
   return thisTransform;
}

void Transform::data(std::vector<double> & v) const
{
   v[ 0] = transformation(0,0);
   v[ 1] = transformation(1,0);
   v[ 2] = transformation(2,0);
   v[ 3] = transformation(3,0);
   v[ 4] = transformation(0,1);
   v[ 5] = transformation(1,1);
   v[ 6] = transformation(2,1);
   v[ 7] = transformation(3,1);
   v[ 8] = transformation(0,2);
   v[ 9] = transformation(1,2);
   v[10] = transformation(2,2);
   v[11] = transformation(3,2);
   v[12] = transformation(0,3);
   v[13] = transformation(1,3);
   v[14] = transformation(2,3);
   v[15] = transformation(3,3);
}

Transform Transform::operator+(const Transform & other) const
{
   double v[16];

   v[ 0] = transformation(0,0) + other.transformation(0,0);
   v[ 1] = transformation(1,0) + other.transformation(1,0);
   v[ 2] = transformation(2,0) + other.transformation(2,0);
   v[ 3] = transformation(3,0) + other.transformation(3,0);
   v[ 4] = transformation(0,1) + other.transformation(0,1);
   v[ 5] = transformation(1,1) + other.transformation(1,1);
   v[ 6] = transformation(2,1) + other.transformation(2,1);
   v[ 7] = transformation(3,1) + other.transformation(3,1);
   v[ 8] = transformation(0,2) + other.transformation(0,2);
   v[ 9] = transformation(1,2) + other.transformation(1,2);
   v[10] = transformation(2,2) + other.transformation(2,2);
   v[11] = transformation(3,2) + other.transformation(3,2);
   v[12] = transformation(0,3) + other.transformation(0,3);
   v[13] = transformation(1,3) + other.transformation(1,3);
   v[14] = transformation(2,3) + other.transformation(2,3);
   v[15] = transformation(3,3) + other.transformation(3,3);

   return Transform(v);
}

Transform Transform::operator*(const Transform & other) const
{
   return Transform(transformation * other.transformation);
}

Transform Transform::operator*(const double s) const
{
   double v[16];

   v[ 0] = transformation(0,0) * s;
   v[ 1] = transformation(1,0) * s;
   v[ 2] = transformation(2,0) * s;
   v[ 3] = transformation(3,0) * s;
   v[ 4] = transformation(0,1) * s;
   v[ 5] = transformation(1,1) * s;
   v[ 6] = transformation(2,1) * s;
   v[ 7] = transformation(3,1) * s;
   v[ 8] = transformation(0,2) * s;
   v[ 9] = transformation(1,2) * s;
   v[10] = transformation(2,2) * s;
   v[11] = transformation(3,2) * s;
   v[12] = transformation(0,3) * s;
   v[13] = transformation(1,3) * s;
   v[14] = transformation(2,3) * s;
   v[15] = transformation(3,3) * s;

   return Transform(v);
}

Transform Transform::interpolate(const Transform & other, double a) const
{
   cg3::Vec3d t1 = getTranslation();
   cg3::Vec3d t2 = other.getTranslation();

   cg3::Vec3d t = (t1 * (1-a)) + (t2 * a);

   //can be optimized!
   Eigen::AngleAxisd r1; r1.fromRotationMatrix(transformation.rotation());
   Eigen::AngleAxisd r2; r2.fromRotationMatrix(other.transformation.rotation());
   Eigen::Quaterniond q1; q1 = r1;
   Eigen::Quaterniond q2; q2 = r2;

   Eigen::Quaterniond q = q1.slerp(a, q2);

   cg3::eigenTransformation T;
   T = q;
   T.translation() = Eigen::Vector3d(t.x(), t.y(), t.z());

   return Transform(T);
}

std::ostream & operator<< (std::ostream & stream, const Transform & t)
{
   std::vector<double> vi;
   t.data(vi);
   std::vector<double> v(16);
   v = vi;
   /*stream << v[ 0] << " " << v [ 1] << " "  << v[ 2] << " "  << v[ 3] << " \n"
          << v[ 4] << " " << v [ 5] << " "  << v[ 6] << " "  << v[ 7] << " \n"
          << v[ 8] << " " << v [ 9] << " "  << v[10] << " "  << v[11] << " \n"
          << v[12] << " " << v [13] << " "  << v[14] << " "  << v[15] << " \n"*/;
   return stream;
}

}
