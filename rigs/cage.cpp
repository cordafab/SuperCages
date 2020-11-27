#include "rigs/cage.h"
#include "geom/plane.h"
#include "iostream"

Cage::Cage()
   : Trimesh()
{

}

Cage::Cage( const std::vector<double>   & vertices ,
            const std::vector<int>      & tris     )
{
   create(vertices, tris);
}

bool Cage::create(const std::vector<double>   & vertices ,
                       const std::vector<int>      & tris     )
{
    clear();    //TODO: Try to solve the duplicate "Trimesh::clear()" call

    Trimesh::create(vertices, tris);

    restPoseVertices = vertices;
    basePoseVertices = vertices;

    init();

    return true;
}

void Cage::init()
{
   restPoseVerticesNormals.resize(vertices.size());
   restPoseTrianglesNormals.resize(tris.size());
   //deformedPoseFrames.resize(getNumVertices());
   //restPoseFrames.resize(getNumVertices());

   updateNormals();

}

void Cage::clear()
{
   restPoseVertices.clear();
   basePoseVertices.clear();
   restPoseVerticesNormals.clear();
   restPoseTrianglesNormals.clear();

   Trimesh::clear();
}

void Cage::interpolateKeyframes(const std::vector<double> & keyframeLow,
                                const std::vector<double> & keyframeTop,
                                double a)
{
   for(unsigned long i=0; i<vertices.size(); ++i)
   {
      restPoseVertices[i] = basePoseVertices[i] + ((keyframeLow[i] * (1.0-a)) + (keyframeTop[i] * a));
   }
}

void Cage::updateNormals()
{
   updateTrisNormals();
   updateVerticesNormals();
}

void Cage::updateTrisNormals()
{
   Trimesh::updateTrisNormals();
   //CODE FROM CAGELAB
   restPoseTrianglesNormals.clear();
   restPoseTrianglesNormals.resize(getNumTriangles()*3);

   for(unsigned long tId=0; tId<getNumTriangles(); ++tId)
   {
      unsigned long tIdPtr = tId * 3;

      cg3::Vec3<double> v0 = getRestPoseVertex(tris[tIdPtr+0]);
      cg3::Vec3<double> v1 = getRestPoseVertex(tris[tIdPtr+1]);
      cg3::Vec3<double> v2 = getRestPoseVertex(tris[tIdPtr+2]);

      cg3::Vec3<double> u = v1 - v0;    u.normalize();
      cg3::Vec3<double> v = v2 - v0;    v.normalize();
      cg3::Vec3<double> n = u.cross(v); n.normalize();

      restPoseTrianglesNormals[tIdPtr + 0] = n.x();
      restPoseTrianglesNormals[tIdPtr + 1] = n.y();
      restPoseTrianglesNormals[tIdPtr + 2] = n.z();
   }
}

void Cage::updateVerticesNormals()
{
   Trimesh::updateVerticesNormals();
   //CODE FROM CAGELAB
   restPoseVerticesNormals.clear();
   restPoseVerticesNormals.resize(getNumVertices()*3);

   for(int vId=0; vId<getNumVertices(); ++vId)
   {
      std::vector<int> nbrs = v2t(vId);

      cg3::Vec3<double> sum(0,0,0);
      for(int i=0; i<(int)nbrs.size(); ++i)
      {
         sum += getRestPoseTriangleNormal(nbrs[i]);
      }

      //TODO: assert(nbrs.size() > 0);
      sum /= nbrs.size();
      sum.normalize();

      int vIdPtr = vId * 3;
      restPoseVerticesNormals[vIdPtr + 0] = sum.x();
      restPoseVerticesNormals[vIdPtr + 1] = sum.y();
      restPoseVerticesNormals[vIdPtr + 2] = sum.z();
   }
}
