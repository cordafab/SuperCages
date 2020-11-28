#include "rigs/cage.h"
#include "geom/plane.h"
#include "iostream"

Cage::Cage()
{

}

Cage::Cage(const std::vector<double> & vertices ,
           const std::vector<int> & tris )
{
   create(vertices, tris);
}

bool Cage::create(const std::vector<double> & vertices ,
                  const std::vector<int> & tris )
{
    clear();

    originalRestPose.create(vertices, tris);

    restPose = originalRestPose;
    currentPose = originalRestPose;

    init();

    return true;
}

void Cage::init()
{
   lastTranslations.resize(originalRestPose.getNumVertices()*3, 0.0);
}

void Cage::clear()
{
   lastTranslations.clear();
}

void Cage::setKeyframe(const std::vector<double> & keyframe)
{

   const std::vector<double> & originalRestPoseVerticesVector = getOriginalRestPoseVertices();
   std::vector<double> & restPoseVerticesVector = restPose.getVerticesVector();

   for(unsigned long i=0; i<originalRestPoseVerticesVector.size(); ++i)
   {
      restPoseVerticesVector[i] = originalRestPoseVerticesVector[i] + keyframe[i];
   }
}

void Cage::interpolateKeyframes(const std::vector<double> & keyframeLow,
                                const std::vector<double> & keyframeTop,
                                double a)
{
   const std::vector<double> & originalRestPoseVerticesVector = getOriginalRestPoseVertices();
   std::vector<double> & restPoseVerticesVector = restPose.getVerticesVector();

   for(unsigned long i=0; i<originalRestPoseVerticesVector.size(); ++i)
   {
      restPoseVerticesVector[i] = originalRestPoseVerticesVector[i] + ((keyframeLow[i] * (1.0-a)) + (keyframeTop[i] * a));
   }
}

