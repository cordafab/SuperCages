#ifndef ANIMATOROPERATIONS_H
#define ANIMATOROPERATIONS_H

#include <vector>
#include "geom/transform.h"
#include "rigs/skeleton.h"

void initializeAnimator();
void clearAnimator();
void addSkelKeyframe();
void addCageKeyframe();
void setSkelKeyframe(unsigned long nodeIndex);
void setCageKeyframe(unsigned long nodeIndex);
void loadSkelAnimationFromFile();
void loadCageAnimationFromFile();
void saveSkelAnimationToFile();
void saveCageAnimationToFile();
void setNextSkelKeyframe();
void setNextCageKeyframe();
void deleteSkelKeyframe(int index);
void deleteCageKeyframe(int index);
void editSkelKeyframeTime(int index);
void editCageKeyframeTime(int index);
void quickLoadSkelAnimation(const char * filename);
void quickLoadCageAnimation(const char * filename);
void convertSkelKeyframes(std::vector<std::vector<cg3::Transform> > & skelKeyframes, Skeleton * skel);

#endif // ANIMATOROPERATIONS_H
