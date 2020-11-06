#ifndef CAGE_H
#define CAGE_H

#include "geom/trimesh.h"
#include "common/types.h"

class Cage
      : public Trimesh
{
public:
   Cage();
   Cage( const std::vector<double>      & vertices ,
         const std::vector<int>         & tris     );

   bool create(const std::vector<double>    & vertices ,
               const std::vector<int>       & tris     );

   void init();
   void clear();

   void updateNormals();

   inline const std::vector<double> & getRestPoseVerticesVector() const
   {
      return restPoseVertices;
   }

   inline const std::vector<double> & getBasePoseVerticesVector() const
   {
      return basePoseVertices;   //needed for the animator
   }

   inline cg3::Vec3<double> getRestPoseVertex(unsigned long vId) const
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      return cg3::Vec3<double>(
                  restPoseVertices[vIdPtr + 0],
                  restPoseVertices[vIdPtr + 1],
                  restPoseVertices[vIdPtr + 2]
             );
   }

   inline void setRestPoseVertex(unsigned long vId, cg3::Vec3d newPosition)
   {
      unsigned long vIdPtr = vId * 3;
      //TODO: CHECK_BOUNDS(coords, vid_ptr+2);
      restPoseVertices[vIdPtr + 0] = newPosition.x();
      restPoseVertices[vIdPtr + 1] = newPosition.y();
      restPoseVertices[vIdPtr + 2] = newPosition.z();
   }

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

   inline void setRestPoseVector(const std::vector<double> _vertices){restPoseVertices = _vertices;}

   inline const std::vector<double> & getActualPoseVerticesVector() const
   {
      return getVerticesVector();
   }

   inline cg3::Vec3<double> getActualPoseVertex(unsigned long vId) const
   {
      return getVertex(vId);
   }

   inline void setActualPoseVector(const std::vector<double> _vertices){setVerticesVector(_vertices);}


   inline void setKeyframe(const std::vector<double> & keyframe)
   {
      for(unsigned long i=0; i<vertices.size(); ++i)
      {
         restPoseVertices[i] = basePoseVertices[i] + keyframe[i];
      }
   }

   void interpolateKeyframes(const std::vector<double> & keyframeLow,
                             const std::vector<double> & keyframeTop,
                             double a);

   inline const std::vector<double> & getLastTranslations() const
      { return lastTranslations; }

protected:

   std::vector<double> restPoseVertices;
   std::vector<double> basePoseVertices;

   void updateTrisNormals();
   void updateVerticesNormals();
   std::vector<double> restPoseVerticesNormals;
   std::vector<double> restPoseTrianglesNormals;

   //TODO: ELIMINA IMPLEMENTAZIONI DA FILE H E CREA UN CPP DEDICATO

   std::vector<double> lastTranslations;

};

#endif // CAGE_H



