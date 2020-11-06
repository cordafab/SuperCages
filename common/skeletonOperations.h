#ifndef SKELETONOPERATIONS_H
#define SKELETONOPERATIONS_H

class Skeleton;

void saveSkelWeightsToFile();
void clearSkeleton();
void updateSkeletonInfluenceTexture(bool renderOnlyTheFather);
void updateSkeletonUpdaterInfluenceTexture();
void switchSkeletonDeformation();
void initializeSkeletonUpdater();
void addFakeBones(Skeleton * skeleton); //Is it necessary?

#endif // SKELETONOPERATIONS_H
