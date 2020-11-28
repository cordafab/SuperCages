#ifndef CHARACTER_H
#define CHARACTER_H

#include "common/types.h"
#include "geom/trimesh.h"

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

   //CAGE POSE

   inline const std::vector<double> & getCagePoseVerticesVector() const
   {
      return cagePoseVertices;
   }

   inline cg3::Vec3<double> getCagePoseVertex(ulong vId) const
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      return cg3::Vec3<double>(
                  cagePoseVertices[vIdPtr + 0],
                  cagePoseVertices[vIdPtr + 1],
                  cagePoseVertices[vIdPtr + 2]
             );
   }

   inline void setCagePoseVertex(ulong vId, cg3::Vec3d newPosition)
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      cagePoseVertices[vIdPtr + 0] = newPosition.x();
      cagePoseVertices[vIdPtr + 1] = newPosition.y();
      cagePoseVertices[vIdPtr + 2] = newPosition.z();
   }

   inline void setCagePoseVector(const std::vector<double> & _vertices){cagePoseVertices = _vertices;}

   //SKELETON POSE

   inline const std::vector<double> & getSkelPoseVerticesVector() const
   {
      return skelPoseVertices;
   }

   inline cg3::Vec3<double> getSkelPoseVertex(ulong vId) const
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      return cg3::Vec3<double>(
                  skelPoseVertices[vIdPtr + 0],
                  skelPoseVertices[vIdPtr + 1],
                  skelPoseVertices[vIdPtr + 2]
             );
   }

   inline void setSkelPoseVertex(ulong vId, cg3::Vec3d newPosition)
   {
      ulong vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      skelPoseVertices[vIdPtr + 0] = newPosition.x();
      skelPoseVertices[vIdPtr + 1] = newPosition.y();
      skelPoseVertices[vIdPtr + 2] = newPosition.z();
   }

   inline void setSkelPoseVector(const std::vector<double> & _vertices){skelPoseVertices = _vertices;}

   //POSE SWAPPING //are they still used?

   inline void setRestPoseAsActual() {setActualPoseVector(restPoseVertices);}
   inline void setCagePoseAsActual() {setActualPoseVector(cagePoseVertices);}
   inline void setSkelPoseAsActual() {setActualPoseVector(skelPoseVertices);}


protected:

   //TODO: ELIMINA IMPLEMENTAZIONI DA FILE H
   std::vector<double> restPoseVertices;
   std::vector<double> restPoseVerticesNormals; //are they still used?
   std::vector<double> restPoseTrianglesNormals; //are they still used?
   std::vector<double> cagePoseVertices; //are they still used?
   std::vector<double> skelPoseVertices; //are they still used?
   std::vector<double> skelCageBlendVertices; //are they still used?

   double blendValue; //are they still used?

   void updateVerticesNormals();
   void updateTrisNormals();
};

#endif // CHARACTER_H
