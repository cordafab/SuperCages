#ifndef TYPES_H
#define TYPES_H

#include "Eigen/Geometry"
#include "Eigen/Core"

namespace cg3
{

typedef Eigen::Transform<double, 3, Eigen::Affine> eigenTransformation;

}

typedef float real;
typedef unsigned long ulong;

#endif // TYPES_H
