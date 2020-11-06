#ifndef DUALQUATERNIONS_H
#define DUALQUATERNIONS_H

#include "geom/vec3.h"
#include "math/quaternion.h"
#include "geom/transform.h"
#include <cmath>

namespace cg3 {

/* Additional infos on http://rodolphe-vaillant.fr/?e=29
 * Thanks Rodolphe
 *
 * TODO:
 * riorganizza il codice per gestire la normalizzazione nei costruttori
 *
 *
 */

template<class T> class DualQuaternion
{
private:

   Quaternion<T> _qReal;  //rotation (<0,0,0>,1) is identity
   Quaternion<T> _qDual;  //translation (<0,0,0>,0) is identity

public:

   DualQuaternion():
      _qReal(), //rotation identity (s=1.0 represents rotation)
      _qDual(0,0,0,0)   //represents the point <0,0,0> (s=0.0 represents point/vec)
   {

   }

   DualQuaternion(const Quaternion<T> & qReal, const Quaternion<T> & qDual)
   {
      _qReal = qReal;
      _qDual = qDual;
   }

   DualQuaternion(const Quaternion<T> & rotation, const Vec3<T> & translation)
   {
      set(rotation, translation);
   }

   DualQuaternion(const Transform & t)
   {
      set(t.getRotation(),t.getTranslation());
   }

   DualQuaternion(const Vec3<T> & axis,
                  const T & angle,
                  const Vec3<T> & translation):
                     _qReal(axis, angle)
   {
      Quaternion<T> t(translation.x()*0.5,
                      translation.y()*0.5,
                      translation.z()*0.5,
                      0);
      _qDual = t * _qReal;
   }

   inline Quaternion<T> & qReal() { return _qReal; }
   inline Quaternion<T> & qDual() { return _qDual; }

   inline const Quaternion<T> & qReal() const { return _qReal; }
   inline const Quaternion<T> & qDual() const { return _qDual; }

   inline void setZero() {_qReal.setZero(); _qDual.setZero();}

   inline void setIdentity()
   {
      _qReal.set(0, 0, 0, 1);
      _qDual.set(0, 0, 0, 0);
   }

   inline void set(const Quaternion<T> & rotation, const Vec3<T> & translation)
   {
      _qReal = rotation;
      //_qReal.normalize();  //to be sure it is a rotation

      Quaternion<T> t(translation.x(),
                      translation.y(),
                      translation.z(),
                      0);
      _qDual = t * _qReal;
      _qDual = _qDual * 0.5;
   }

   inline void setRotationTranslation(const Vec3<T> & axis,
                                      const T & angle,
                                      const Vec3<T> & translation)
   {
      _qReal.setRotation(axis, angle);

      Quaternion<T> t(translation.x(),
                      translation.y(),
                      translation.z(),
                      0);
      _qDual = t * _qReal;
      _qDual = _qDual * 0.5;
   }

   inline T dot(const DualQuaternion<T> & other)
   {
      return _qReal.dot(other._qReal);
   }

   inline DualQuaternion<T> conjugate() const
   {
      return DualQuaternion<T>(_qReal.conjugate(), _qDual.conjugate());
   }

   inline DualQuaternion<T> normalize() const
   {
      T mag = _qReal.magnitude();
      mag = 1.0 / mag;
      return DualQuaternion<T>(_qReal * mag, _qDual * mag);

      //Alternative formulation
      //Quaternion<T> nqr = _qReal * mag;
      //Quaternion<T> nqd = _qDual * mag;
      //WHY?!
      //return DualQuaternion<T>(nqr, nqd - (nqd.dot(nqr) * nqr));
   }

   inline Vec3<T> applyTransformation(const Vec3<T> & v) const
   {
      /*Vec3<T> displacement =
            2.0 * (
                 _qReal.v().cross( _qReal.v().cross(v) + v*_qReal.s() )
               + _qDual.v() * _qReal.s()
               - _qReal.v() * _qReal.s()
               + _qReal.v().cross(_qDual.v())
            );*/

      //Alternative formulation
      Vec3<T> displacement =
            2.0 * (
               _qReal.v().cross(_qReal.v().cross(v) + (v * _qReal.s()) + _qDual.v())
               + _qDual.v() * _qReal.s()
               - _qReal.v() * _qDual.s()
               ); //dqtar

      return v + displacement;
   }

};

template<typename T>
DualQuaternion<T> operator+(const DualQuaternion<T> & a, const DualQuaternion<T> & b)
{
   Quaternion<T> qRealNew = a.qReal() + b.qReal();
   Quaternion<T> qDualNew = a.qDual() + b.qDual();

   return DualQuaternion<T>(qRealNew, qDualNew);
}

template<typename T>
DualQuaternion<T> operator*(const DualQuaternion<T> & a, const DualQuaternion<T> & b)
{
   Quaternion<T> qRealNew = b.qReal() * a.qReal();
   Quaternion<T> qDualNew = (b.qDual()*a.qReal()) + (b.qReal() * a.qDual());

   return DualQuaternion<T>(qRealNew, qDualNew);
}

template<typename T>
DualQuaternion<T> operator*(const DualQuaternion<T> & a, T s)
{
   DualQuaternion<T> ret(s*a.qReal(), s*a.qDual());
   return ret;
}

template<typename T>
DualQuaternion<T> operator*(T s, const DualQuaternion<T> & a)
{
   DualQuaternion<T> ret(s*a.qReal(), s*a.qDual());
   return ret;
}

typedef DualQuaternion<double> dDualQuaternion;

}

#endif // DUALQUATERNIONS_H
