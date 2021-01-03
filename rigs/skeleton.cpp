#include "skeleton.h"

#include <stack>

SkeletonNode::SkeletonNode(      std::string      nodeName,
                                 int              father,
                           const cg3::Transform & localT,
                           const cg3::Transform & globalT)

   : localTCurrent(localT),
     globalTCurrent(globalT),
     localTRest(localT),
     globalTRest(globalT)
{
   this->nodeName = nodeName;
   this->father = father;
   this->next.clear();
}

Skeleton::Skeleton()
{

}

Skeleton::Skeleton(const std::vector<cg3::Vec3d>     & joints,
                   const std::vector<cg3::Vec3d>     & jointsRotations,
                   const std::vector<int>            & fathers,
                   const std::vector<std::string>    & names)
{
   create(joints, jointsRotations, fathers, names);
}

bool Skeleton::create(const std::vector<cg3::Vec3d>     & joints,
                      const std::vector<cg3::Vec3d>     & jointsRotations,
                      const std::vector<int>            & fathers,
                      const std::vector<std::string>    & names)
{
   clear();

   for(unsigned long i = 0; i<joints.size(); ++i)
   {
      double x = joints[i].x();
      double y = joints[i].y();
      double z = joints[i].z();
      double rx = jointsRotations[i].x();
      double ry = jointsRotations[i].y();
      double rz = jointsRotations[i].z();

      addNode(names[i], fathers[i], cg3::Transform(), cg3::Transform(rx, ry, rz, x, y, z));
   }

   updateLocalFromGlobalRest();
   updateLocalFromGlobalCurrent();

   return true;
}

void Skeleton::clear()
{
   rootIndexes.clear();
   nodes.clear();
   boundingBox.clear();
   rootMotion = cg3::Transform();
}

int Skeleton::addNode(std::string nodeName,
                      int father,
                      const cg3::Transform & localTransformation,
                      const cg3::Transform & modelTransformation)
{
   SkeletonNode node(nodeName,
             father,
             localTransformation,
             modelTransformation);

   int id = getNumNodes();

   nodes.push_back(node);

   if(father != -1)
      setNextOnNode(father, id);
   else
      addNodeAsRoot(id);

   updateBoundingBox();

   return id;
}


void Skeleton::addGlobalTransformation(int nodeIndex, const cg3::Transform & transformation)
//It's buggy!
{
   nodes[nodeIndex].globalTCurrent =
         nodes[nodeIndex].globalTCurrent.transformTowardsOrigin(transformation);

   int fatherIndex = nodes[nodeIndex].getFather();

   if(fatherIndex != -1)
   {
      cg3::Transform fatherTransformation = nodes[fatherIndex].globalTCurrent;

      nodes[nodeIndex].localTCurrent =
            fatherTransformation.inverse() * nodes[nodeIndex].globalTCurrent;
   }
   else
   {
      nodes[nodeIndex].localTCurrent =
            nodes[nodeIndex].globalTCurrent;
   }

   for(int i:nodes[nodeIndex].next)
   {
      updateGlobalFromLocalCurrent();
      updateGlobalFromLocalRest();
   }
}

void Skeleton::setKeyframe(const std::vector<cg3::Transform> & keyframe)
{
   for(unsigned long i = 0; i < nodes.size(); ++i)
   {
      nodes[i].localTCurrent = keyframe[i];
   }

   updateGlobalFromLocalCurrent();

   for(int i:rootIndexes)
   {
      cg3::Vec3d t = (nodes[i].getGlobalTCurrent().getTranslation()-
                      nodes[i].getGlobalTRest().getTranslation());
      rootMotion = cg3::Transform(t.x(),t.y(),t.z());
   }
}

void Skeleton::interpolateKeyframes(const std::vector<cg3::Transform> & keyframeLow,
                                    const std::vector<cg3::Transform> & keyframeTop,
                                    double a)
{
   for(unsigned long i = 0; i < nodes.size(); ++i)
   {
      nodes[i].localTCurrent = keyframeLow[i].interpolate(keyframeTop[i],a);
   }

   updateGlobalFromLocalCurrent();

   for(int i:rootIndexes)
   {
      cg3::Vec3d t = (nodes[i].getGlobalTCurrent().getTranslation()-
                      nodes[i].getGlobalTRest().getTranslation());
      rootMotion = cg3::Transform(t.x(),t.y(),t.z());
   }
}

void Skeleton::updateBoundingBox()
{
   //CODE FROM CAGELAB
   boundingBox.clear();
   for(ulong id=0; id<getNumNodes(); ++id)
   {
      cg3::Vec3d v = getNode(id).getGlobalTCurrent().getTranslation();
      boundingBox.min = boundingBox.min.min(v);
      boundingBox.max = boundingBox.max.max(v);
   }
}

void Skeleton::updateLocalFromGlobalRest()
{
   std::stack<int> stack;

   for(int n:rootIndexes)
   {
      stack.push(n);
   }

   while (!stack.empty())
   {
      const int n = stack.top();
      stack.pop();
      SkeletonNode & node = nodes[n];
      const int fatherIndex = node.getFather();

      if(fatherIndex != -1)
      {
         const cg3::Transform & restFatherTransformation = nodes[fatherIndex].globalTRest;
         nodes[n].localTRest =
               nodes[n].globalTRest * restFatherTransformation.inverse();
      }
      else
      {
         nodes[n].localTRest =
               nodes[n].globalTRest;
      }

      const std::vector<ulong> & childs = node.getNext();
      for(ulong child:childs)
      {
         stack.push(child);
      }
   }
}

void Skeleton::updateLocalFromGlobalCurrent()
{
   std::stack<int> stack;

   for(int n:rootIndexes)
   {
      stack.push(n);
   }

   while (!stack.empty())
   {
      const int n = stack.top();
      stack.pop();
      SkeletonNode & node = nodes[n];
      const int fatherIndex = node.getFather();

      if(fatherIndex != -1)
      {
         const cg3::Transform & fatherTransformation = nodes[fatherIndex].globalTCurrent;

         nodes[n].localTCurrent =
               nodes[n].globalTCurrent * fatherTransformation.inverse();
      }
      else
      {
         nodes[n].localTCurrent =
               nodes[n].globalTCurrent;
      }

      const std::vector<ulong> & childs = node.getNext();
      for(ulong child:childs)
      {
         stack.push(child);
      }
   }
}

void Skeleton::updateGlobalFromLocalRest()
{
   std::stack<int> stack;
   for(int n:rootIndexes)
   {
      stack.push(n);
   }

   while (!stack.empty())
   {
      const int n = stack.top();
      stack.pop();
      SkeletonNode & node = nodes[n];
      const int fatherIndex = node.getFather();

      if(fatherIndex != -1)
      {
         const cg3::Transform & restFatherTransformation = nodes[fatherIndex].globalTRest;
         nodes[n].globalTRest =
               restFatherTransformation * nodes[n].localTRest;
      }
      else
      {
         nodes[n].globalTRest =
               nodes[n].localTRest;
      }

      const std::vector<ulong> & childs = node.getNext();
      for(ulong child:childs)
      {
         stack.push(child);
      }
   }
}

void Skeleton::updateGlobalFromLocalCurrent()
{
   std::stack<int> stack;

   for(int n:rootIndexes)
   {
      stack.push(n);
   }

   while (!stack.empty())
   {
      const int n = stack.top();
      stack.pop();
      SkeletonNode & node = nodes[n];
      const int fatherIndex = node.getFather();

      if(fatherIndex != -1)
      {
         const cg3::Transform & fatherTransformation = nodes[fatherIndex].globalTCurrent;

         nodes[n].globalTCurrent =
               fatherTransformation * nodes[n].localTCurrent;
      }
      else
      {
         nodes[n].globalTCurrent =
               nodes[n].localTCurrent;
      }

      const std::vector<ulong> & childs = node.getNext();
      for(ulong child:childs)
      {
         stack.push(child);
      }
   }
}

void Skeleton::updateGlobalT()
{
   for(ulong i=0; i < getNumNodes(); ++i)
   {
      const cg3::Transform & TRest = nodes[i].getGlobalTRest();
      const cg3::Transform & TCurrent = nodes[i].getGlobalTCurrent();
      nodes[i].globalT = TCurrent * TRest.inverse();
   }
}
