/********************************************************************************
*  This file is part of CinoLib                                                 *
*  Copyright(C) 2016: Marco Livesu                                              *
*                                                                               *
*  The MIT License                                                              *
*                                                                               *
*  Permission is hereby granted, free of charge, to any person obtaining a      *
*  copy of this software and associated documentation files (the "Software"),   *
*  to deal in the Software without restriction, including without limitation    *
*  the rights to use, copy, modify, merge, publish, distribute, sublicense,     *
*  and/or sell copies of the Software, and to permit persons to whom the        *
*  Software is furnished to do so, subject to the following conditions:         *
*                                                                               *
*  The above copyright notice and this permission notice shall be included in   *
*  all copies or substantial portions of the Software.                          *
*                                                                               *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
*  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
*  IN THE SOFTWARE.                                                             *
*                                                                               *
*  Author(s):                                                                   *
*                                                                               *
*     Marco Livesu (marco.livesu@gmail.com)                                     *
*     http://pers.ge.imati.cnr.it/livesu/                                       *
*                                                                               *
*     Italian National Research Council (CNR)                                   *
*     Institute for Applied Mathematics and Information Technologies (IMATI)    *
*     Via de Marini, 6                                                          *
*     16149 Genoa,                                                              *
*     Italy                                                                     *
*********************************************************************************/
#ifndef CINO_CUT_ALONG_SEAMS_H
#define CINO_CUT_ALONG_SEAMS_H

#include <sys/types.h>
#include <vector>
#include <_external/cinolib/cino_inline.h>
#include <_external/cinolib/geometry/vec3.h>

namespace cinolib
{

/* These methods serve to switch from separated representations for
 * each vertex property (position, texture coordinates, normals) to
 * an unified (OpenGL-like) vertex representation, where position,
 * texture coordinates and normals are all attributes of the same entity
 *
 * https://stackoverflow.com/questions/29867926/why-does-the-number-of-vt-and-v-elements-in-a-blender-obj-file-differ
*/

CINO_INLINE
void cut_mesh_along_seams(const std::vector<vec3d>             & v_attr_0,          // xyz or uvw or nor
                          const std::vector<vec3d>             & v_attr_1,          // xyz or uvw or nor
                          const std::vector<std::vector<uint>> & v2v_v_attr_0,      // connectivity attr #0
                          const std::vector<std::vector<uint>> & v2v_v_attr_1,      // connectivity attr #1
                                std::vector<vec3d>             & unified_v_attr_0,
                                std::vector<vec3d>             & unified_v_attr_1,
                                std::vector<std::vector<uint>> & unified_v2v);

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

CINO_INLINE
void cut_mesh_along_seams(const std::vector<vec3d>             & v_attr_0,
                          const std::vector<vec3d>             & v_attr_1,
                          const std::vector<vec3d>             & v_attr_2,
                          const std::vector<std::vector<uint>> & v2v_attr_0,
                          const std::vector<std::vector<uint>> & v2v_attr_1,
                          const std::vector<std::vector<uint>> & v2v_attr_2,
                                std::vector<vec3d>             & unified_v_attr_0,
                                std::vector<vec3d>             & unified_v_attr_1,
                                std::vector<vec3d>             & unified_v_attr_2,
                                std::vector<std::vector<uint>> & unified_v2v);
}

#ifndef  CINO_STATIC_LIB
#include "cut_along_seams.cpp"
#endif

#endif // CINO_CUT_ALONG_SEAMS_H
