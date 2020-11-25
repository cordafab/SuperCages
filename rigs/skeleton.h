#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <string>

#include "common/types.h"

#include "geom/transform.h"
#include "geom/boundingBox.h"

class Node
{

   friend class Skeleton;

public:

   Node(      std::string      nodeName,
              int              father,
        const cg3::Transform & localT,
        const cg3::Transform & globalT);


   inline       int                   getFather()              const { return father;         }
   inline       std::string           getNodeName()            const { return nodeName;       }
   inline const std::vector<ulong> &  getNext()                const { return next;           }


// inline const cg3::Point3d       &  getjointPosition()       const { return jointPosition;  }
// inline const cg3::Transform     &  getBoneRotation()        const { return boneRotation;   }

// inline       cg3::Point3d       &  getjointPosition()             { return jointPosition;  }
// inline       cg3::Transform     &  getBoneRotation()              { return boneRotation;   }


   inline const cg3::Transform     &  getLocalTCurrent()       const { return localTCurrent;  }
   inline const cg3::Transform     &  getGlobalTCurrent()      const { return globalTCurrent; }

   inline const cg3::Transform     &  getLocalTRest()          const { return localTRest;     }
   inline const cg3::Transform     &  getGlobalTRest()         const { return globalTRest;    }

// inline const cg3::Transform     &  getLocalT()              const { return localT;         }
   inline const cg3::Transform     &  getGlobalT()             const { return globalT;        }


   inline       cg3::Transform     &  getLocalTCurrent()             { return localTCurrent;  }
   inline       cg3::Transform     &  getGlobalTCurrent()            { return globalTCurrent; }

   inline       cg3::Transform     &  getLocalTRest()                { return localTRest;     }
   inline       cg3::Transform     &  getGlobalTRest()               { return globalTRest;    }

// inline       cg3::Transform     &  getLocalT()                    { return localT;         }
   inline       cg3::Transform     &  getGlobalT()                   { return globalT;        }

protected:

   std::string         nodeName;  //Name of the node
   int                 father;    //Index of the father node
   std::vector<ulong>  next;      //Index of the attached nodes

// cg3::Point3d        jointPosition;
// cg3::Transform      boneRotation;

//Transformations
   cg3::Transform      localTCurrent;
   cg3::Transform      globalTCurrent;

   cg3::Transform      localTRest;
   cg3::Transform      globalTRest;

//T is the transformation that goes from TRest to TCurrent
// cg3::Transform      localT; //Is this equal to boneRotation? Only if applied from the root to the top
   cg3::Transform      globalT;
};


class Skeleton
{
public:

   Skeleton();
   Skeleton(const std::vector<cg3::Vec3d>     & joints, const std::vector<cg3::Vec3d> & jointsRotations,
            const std::vector<int>            & fathers,
            const std::vector<std::string>    & names);

   bool create(const std::vector<cg3::Vec3d>     & joints,
               const std::vector<cg3::Vec3d> & jointsRotations,
               const std::vector<int>            & fathers,
               const std::vector<std::string>    & names);

   void clear();

protected:

   std::vector<int>     rootIndexes;
   std::vector<Node>    nodes;
   BoundingBox          boundingBox;
   cg3::Transform       rootMotion;


   void propagatePose(int nodeIndex); //TO DELETE
   int  addNode(std::string nodeName,
                int father,
                const cg3::Transform & localTransformation,
                const cg3::Transform & modelTransformation);

   void updateBoundingBox();

public:

   inline void setNodesVector(std::vector<Node> & newNodes) { nodes = newNodes; }

   inline const std::vector<int>    & getRootsIndexes()     const { return rootIndexes; }
   inline const std::vector<Node>   & getNodesVector()      const { return nodes; }

   inline const cg3::Transform      & getRootMotion()       const { return rootMotion; }

   inline       unsigned long         getNumRoots()         const { return rootIndexes.size(); }
   inline       unsigned long         getNumNodes()         const { return nodes.size(); }

   inline const Node                & getNode(ulong index)  const { return nodes[index]; }
   inline       Node                & getNode(ulong index)        { return nodes[index]; }

   inline       void                  resetRootMotion()           { rootMotion.setTranslation
                                                                    (cg3::Vec3d(0.0,0.0,0.0)); }


   inline void addNodeAsRoot(int index) { rootIndexes.push_back(index); }
   inline void setNextOnNode(ulong index, const ulong nextIndex)
                                        { nodes[index].next.push_back(nextIndex); }

   void updateLocalFromGlobalRest();
   void updateLocalFromGlobalCurrent();

   void updateGlobalFromLocalRest();
   void updateGlobalFromLocalCurrent();

   void updateGlobalT();   //compute the globalT transformations
// void updateLocalT();

   void setKeyframe(const std::vector<cg3::Transform> & keyframe);
   void interpolateKeyframes(const std::vector<cg3::Transform> & keyframeLow,
                              const std::vector<cg3::Transform> & keyframeTop,
                              double a);

   //TO DO: CLEAN THIS
   void addGlobalTransformation(int nodeIndex, const cg3::Transform & transformation);

   //void setGlobalNodePosition(unsigned long nodeIndex, const cg3::Vec3d & position);
   //void setGlobalRestNodePosition(unsigned long nodeIndex, const cg3::Vec3d & position);

   //cg3::Vec3d getLocalNodePosition(unsigned long nodeIndex);
   //void setLocalNodePosition(unsigned long nodeIndex, const cg3::Vec3d & position);

   //void updateLocalTransformations(int nodeIndex);
};

#endif // SKELETON_H
