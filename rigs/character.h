#ifndef CHARACTER_H
#define CHARACTER_H

#include "common/types.h"
#include "mesh/trimesh.h"

class Character
      : public Trimesh
{
public:
   Character();
   Character(const std::vector<double>      & vertices ,
             const std::vector<int>         & tris     );

   bool create(const std::vector<double>    & vertices ,
               const std::vector<int>       & tris     );

   void init();
   void clear();

   void updateNormals();

   //REST POSE

   inline std::vector<double> & getRestPoseVerticesVector()
   {
      return restPoseVertices;
   }

   inline const std::vector<double> & getRestPoseVerticesVector() const
   {
      return restPoseVertices;
   }

   inline cg3::Vec3<double> getRestPoseVertex(ulong vId) const
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      return cg3::Vec3<double>(
                  restPoseVertices[vIdPtr + 0],
                  restPoseVertices[vIdPtr + 1],
                  restPoseVertices[vIdPtr + 2]
             );
   }

   inline void setRestPoseVertex(ulong vId, cg3::Vec3d newPosition)
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      restPoseVertices[vIdPtr + 0] = newPosition.x();
      restPoseVertices[vIdPtr + 1] = newPosition.y();
      restPoseVertices[vIdPtr + 2] = newPosition.z();
   }

   inline void setRestPoseVector(const std::vector<double> & _vertices){restPoseVertices = _vertices;}

   //ACTUAL POSE

   inline std::vector<double> & getActualPoseVerticesVector()
   {
      return vertices;
   }

   inline const std::vector<double> & getActualPoseVerticesVector() const
   {
      return getVerticesVector();
   }

   inline cg3::Vec3<double> getActualPoseVertex(ulong vId) const
   {
      return getVertex(vId);
   }

   inline void setActualPoseVector(const std::vector<double> & _vertices){setVerticesVector(_vertices);}

   inline cg3::Vec3<double> getRestPoseVertexNormal(unsigned long vId) const
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(v_norm, vid_ptr+2);
      return cg3::Vec3<double>(
                  restPoseVerticesNormals[vIdPtr + 0],
                  restPoseVerticesNormals[vIdPtr + 1],
                  restPoseVerticesNormals[vIdPtr + 2]
             );
   }

   inline cg3::Vec3<double> getRestPoseTriangleNormal(unsigned long tId) const
   {
      unsigned long tIdPtr = tId * 3;
      //TODO: CHECK_BOUNDS(v_norm, vid_ptr+2);
      return cg3::Vec3<double>(
                  restPoseTrianglesNormals[tIdPtr + 0],
                  restPoseTrianglesNormals[tIdPtr + 1],
                  restPoseTrianglesNormals[tIdPtr + 2]
             );
   }

protected:

   //TODO: ELIMINA IMPLEMENTAZIONI DA FILE H
   std::vector<double> restPoseVertices;
   std::vector<double> restPoseVerticesNormals;
   std::vector<double> restPoseTrianglesNormals;

   void updateVerticesNormals();
   void updateTrisNormals();
};

#endif // CHARACTER_H
