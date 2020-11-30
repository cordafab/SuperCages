#ifndef SKELETON_H
#define SKELETON_H

#include <vector>
#include <string>

#include "common/types.h"
#include "geom/transform.h"
#include "geom/boundingBox.h"

class SkeletonNode
{
   friend class Skeleton;

public:

   SkeletonNode(      std::string      nodeName,
                      int              father,
                const cg3::Transform & localT,
                const cg3::Transform & globalT);

protected:

   std::string         nodeName;  //Name of the node
   int                 father;    //Index of the father node
   std::vector<ulong>  next;      //Index of the attached nodes

//Transformations
   cg3::Transform      localTCurrent;
   cg3::Transform      globalTCurrent;

   cg3::Transform      localTRest;
   cg3::Transform      globalTRest;


   // cg3::Transform      localT;
   cg3::Transform      globalT; //TO DO: Rename it "finalT"

public:

   inline       int                   getFather()              const { return father;         }
   inline       std::string           getNodeName()            const { return nodeName;       }
   inline const std::vector<ulong> &  getNext()                const { return next;           }

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
};


class Skeleton
{
public:

   Skeleton();
   Skeleton(const std::vector<cg3::Vec3d>     & joints,
            const std::vector<cg3::Vec3d>     & jointsRotations,
            const std::vector<int>            & fathers,
            const std::vector<std::string>    & names);

   bool create(const std::vector<cg3::Vec3d>     & joints,
               const std::vector<cg3::Vec3d>     & jointsRotations,
               const std::vector<int>            & fathers,
               const std::vector<std::string>    & names);

   void clear();

protected:

   std::vector<int>             rootIndexes;
   std::vector<SkeletonNode>    nodes;
   BoundingBox                  boundingBox;

   int addNode(std::string nodeName,
               int father,
               const cg3::Transform & localTransformation,
               const cg3::Transform & modelTransformation);

   inline void addNodeAsRoot(int index) { rootIndexes.push_back(index); }

   inline void setNextOnNode(      ulong index,
                             const ulong nextIndex)
                             { nodes[index].next.push_back(nextIndex); }

   void updateBoundingBox();


   //TO DO: MOVE IT INTO SKELETON UPDATER
   cg3::Transform               rootMotion;

public:

   inline       unsigned long                 getNumRoots()         const { return rootIndexes.size(); }
   inline       unsigned long                 getNumNodes()         const { return nodes.size(); }

   inline const std::vector<int>            & getRootsIndexes()     const { return rootIndexes; }

   inline const std::vector<SkeletonNode>   & getNodesVector()      const { return nodes; }
   inline       std::vector<SkeletonNode>   & getNodesVector()            { return nodes; }

   inline const SkeletonNode                & getNode(ulong index)  const { return nodes[index]; }
   inline       SkeletonNode                & getNode(ulong index)        { return nodes[index]; }

   void updateLocalFromGlobalRest();
   void updateLocalFromGlobalCurrent();

   void updateGlobalFromLocalRest();
   void updateGlobalFromLocalCurrent();

   void updateGlobalT();



   //TO DO: MOVE IT INTO SKELETON UPDATER
   inline const cg3::Transform              & getRootMotion()       const { return rootMotion; }

   //TO DO: MOVE IT INTO SKELETON UPDATER
   inline void resetRootMotion() { rootMotion.setTranslation(cg3::Vec3d(0.0,0.0,0.0)); }

   //TO DO: MOVE IT INTO ANIMATOR
   void setKeyframe(const std::vector<cg3::Transform> & keyframe);

   //TO DO: MOVE IT INTO ANIMATOR
   void interpolateKeyframes(const std::vector<cg3::Transform> & keyframeLow,
                             const std::vector<cg3::Transform> & keyframeTop,
                             double a);

   //TO DO: CLEAN THIS
   void addGlobalTransformation(int nodeIndex, const cg3::Transform & transformation);
};

#endif // SKELETON_H
