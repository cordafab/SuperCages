#ifndef TRIMESH_H
#define TRIMESH_H

//// V0
//coords[0]  = 0.1;
//coords[1]  = 0.2;
//coords[2]  = 0.3;
//// V1
//coords[3]  = 0.1;
//coords[4]  = 0.45876;
//coords[5]  = 0.253;
//// V2
//coords[6]  = 0.1;
//coords[7]  = 0.45876;
//coords[8]  = 0.253;
//// V3
//coords[9]  = 0.1;
//coords[10]  = 0.45876;
//coords[11]  = 0.253;
//// V4
//coords[12]  = 0.1;
//coords[13]  = 0.45876;
//coords[14]  = 0.253;

//vid = 12;
//coords[12*3+0]; // X di V12
//coords[12*3+1]; // Y di V12
//coords[12*3+2]; // Z di V12

//// T0
//tris[0] = 0;
//tris[1] = 2;
//tris[2] = 3;
//// T1
//tris[3] = 1;
//tris[4] = 4;
//tris[5] = 5;

//using namespace std;

#include <vector>
#include <set>
#include <map>
#include "geom/boundingBox.h"
#include "geom/edge.h"
#include "common/types.h"
#include "Eigen/Core"


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

   //TODO: decidi il modo di richiamare l'init in modo intelligente
   inline void setVerticesVector (const std::vector<double> _vertices)  {vertices = _vertices;}
   inline void setTrianglesVector(const std::vector<int> _tris)      {tris = _tris;}

   inline unsigned long getNumVertices()   const { return vertices.size()/3;  }
   inline unsigned long getNumTriangles()  const { return tris.size()/3;    }

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
   void exportVerticesToEigen(Eigen::VectorXd & vx, Eigen::VectorXd & vy, Eigen::VectorXd & vz);

};

#endif // TRIMESH_H
