#ifndef TRIMESH_H
#define TRIMESH_H

#include <vector>
#include <set>
#include <map>

#include "common/types.h"
#include "geom/boundingBox.h"
#include "geom/edge.h"

#ifdef EIGEN_AVAILABLE
#include "Eigen/Core"
#endif


class Trimesh
{
public:

   Trimesh();
   Trimesh(const std::vector<double>     & _vertices ,
           const std::vector<int>        & _tris     );

protected:

   std::vector<double>                vertices;
   std::vector<int>                   tris;
   std::vector<double>                verticesNorm;
   std::vector<double>                trisNorm;
   std::vector<std::vector<int>>      _v2t;
   std::vector<std::vector<int>>      _v2v;
   std::vector<std::vector<int>>      _t2t;
   BoundingBox                        boundingBox;

   void clear();
   void init();
   void buildAdjacency();
   void updateTrisNormals();
   void updateVerticesNormals();

public:

   bool create(const std::vector<double>     & _vertices ,
               const std::vector<int>        & _tris     );

   inline const std::vector<double>     & getVerticesVector()       const { return vertices; }
   inline const std::vector<int>        & getTrianglesVector()      const { return tris;   }

   inline       std::vector<double>     & getVerticesVector()             { return vertices; }
   inline       std::vector<int>        & getTrianglesVector()            { return tris;   }

   //TODO: decidi il modo di richiamare l'init in modo intelligente
   inline void setVerticesVector (const std::vector<double> & _vertices)  {vertices = _vertices;}
   inline void setTrianglesVector(const std::vector<int> & _tris)         {tris = _tris;}

   inline ulong getNumVertices()   const { return vertices.size()/3;  }
   inline ulong getNumTriangles()  const { return tris.size()/3;    }

   //TODO: AGGIUNGI I CHECK_BOUNDS PER VERIFICARE CORRETTEZZA INDICI
   inline const std::vector<int> & v2t(unsigned long vId) const { return _v2t[vId]; }
   inline const std::vector<int> & v2v(unsigned long vId) const { return _v2v[vId]; }
   inline const std::vector<int> & t2t(unsigned long tId) const { return _t2t[tId]; }

   inline cg3::Vec3<double> getTriangleNormal(unsigned long tId) const
   {
      unsigned long tIdPtr = tId * 3;
      //TODO: CHECK_BOUNDS(t_norm, tid_ptr+2);
      return cg3::Vec3<double>(
                  trisNorm[tIdPtr + 0],
                  trisNorm[tIdPtr + 1],
                  trisNorm[tIdPtr + 2]
             );
   }

   inline cg3::Vec3<double> getVertexNormal(unsigned long vId) const
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(v_norm, vid_ptr+2);
      return cg3::Vec3<double>(
                  verticesNorm[vIdPtr + 0],
                  verticesNorm[vIdPtr + 1],
                  verticesNorm[vIdPtr + 2]
             );
   }

   inline cg3::Vec3<double> getVertex(unsigned long vId) const
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      return cg3::Vec3<double>(
                  vertices[vIdPtr + 0],
                  vertices[vIdPtr + 1],
                  vertices[vIdPtr + 2]
             );
   }

   inline void setVertex(unsigned long vId, cg3::Vec3d newPosition)
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      vertices[vIdPtr + 0] = newPosition.x();
      vertices[vIdPtr + 1] = newPosition.y();
      vertices[vIdPtr + 2] = newPosition.z();
   }

   void updateNormals();
   void updateBoundingBox();

#ifdef EIGEN_AVAILABLE
   void exportVerticesToEigen(Eigen::VectorXd & vx, Eigen::VectorXd & vy, Eigen::VectorXd & vz);
#endif

   inline BoundingBox getBoundingBox() const { return boundingBox; }

};

#endif // TRIMESH_H
