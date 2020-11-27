#ifndef CAGE_H
#define CAGE_H

#include "geom/trimesh.h"
#include "common/types.h"

class Cage
{
public:
   Cage();
   Cage(const std::vector<double> & vertices ,
        const std::vector<int> & tris        );

   bool create(const std::vector<double> & vertices ,
               const std::vector<int> & tris        );

   void init();
   void clear();

protected:

   //TO DO: Evaluate a simpler and less memory redundant data structure than Trimesh for the three poses?
   /*Note to self. Remeber that:
    * - a cage can be also composed of quads (see QMVC)
    * - a cage may need info about it's normals (see Green Coords) */
   Trimesh originalRestPose;
   Trimesh restPose;
   Trimesh currentPose;

public:

   inline ulong getNumVertices() const
   {
      return originalRestPose.getNumVertices();
   }

   inline int getNumTriangles() const
   {
      return originalRestPose.getNumTriangles();
   }


   //Current Pose encapsulation methods

   inline const std::vector<double> & getCurrentPoseVertices() const
   {
      return currentPose.getVerticesVector();
   }

   inline void setCurrentPoseVector(const std::vector<double> & _vertices)
   {
      currentPose.setVerticesVector(_vertices);
   }

   inline const std::vector<int> & getCurrentPoseTriangles() const
   {
      return currentPose.getTrianglesVector();
   }

   inline cg3::Vec3<double> getCurrentPoseVertex(unsigned long vId) const
   {
      return currentPose.getVertex(vId);
   }

   inline void updateCurrentPoseNormals()
   {
      currentPose.updateNormals();
   }


   //Rest Pose encapsulation methods

   inline const std::vector<double> & getRestPoseVertices() const
   {
      return restPose.getVerticesVector();
   }

   inline void setRestPoseVector(const std::vector<double> & _vertices)
   {
      restPose.setVerticesVector(_vertices);
   }

   inline cg3::Vec3<double> getRestPoseVertex(unsigned long vId) const
   {
      return restPose.getVertex(vId);
   }

   inline void setRestPoseVertex(unsigned long vId, cg3::Vec3d newPosition)
   {
      restPose.setVertex(vId, newPosition);
   }

   inline cg3::Vec3<double> getRestPoseVertexNormal(unsigned long vId) const
   {
      return restPose.getVertexNormal(vId);
   }

   inline cg3::Vec3<double> getRestPoseTriangleNormal(unsigned long tId) const
   {
      return restPose.getTriangleNormal(tId);
   }

   inline void updateRestPoseNormals()
   {
      restPose.updateNormals();
   }


   //Original Rest Pose encapsulation methods

   inline const std::vector<double> & getOriginalRestPoseVertices() const
   {
      return originalRestPose.getVerticesVector();
   }




   //TO DO: MOVE TO CAGE ANIMATOR
   void setKeyframe(const std::vector<double> & keyframe);

   //TO DO: MOVE TO CAGE ANIMATOR
   void interpolateKeyframes(const std::vector<double> & keyframeLow,
                             const std::vector<double> & keyframeTop,
                             double a);

    //TO DO: MOVE TO CAGE REVERSER
   inline const std::vector<double> & getLastTranslations() const
      { return lastTranslations; }

protected:

   //TO DO: MOVE TO CAGE REVERSER?
   std::vector<double> lastTranslations;
};

#endif // CAGE_H



