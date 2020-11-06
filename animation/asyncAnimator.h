#ifndef ASYNCANIMATOR_H
#define ASYNCANIMATOR_H

#include <QTime>

#include "rigs/cage.h"
#include "rigs/skeleton.h"
#include "geom/transform.h"


class AsyncAnimator
{
public:
   AsyncAnimator();
   AsyncAnimator(Cage * _cage, Skeleton * _skel);

   bool create(Cage * _cage, Skeleton * _skel);

   void clear();

   void loadAnimation(
                      const std::vector<double> & _tCage,
                      const std::vector<double> & _tSkel,
                      const std::vector<std::vector<double>> & _cageKeyframes,
                      const std::vector<std::vector<cg3::Transform>> & _skelKeyframes
                      );

   void addCageKeyframe(double t,
                        const std::vector<double> & cageKeyframe);
   void addSkelKeyframe(double t,
                        const std::vector<cg3::Transform> & skelKeyframe);

   void rewindAnimator();

   void updateFrame();
   void setSkeletonFrame(double t);
   void setCageFrame(double t);
   void setSkeletonKeyframeIndex(unsigned long frameIndex);
   void setCageKeyframeIndex(unsigned long frameIndex);
   void deleteSkelKeyframe(unsigned long frameIndex);
   void deleteCageKeyframe(unsigned long frameIndex);
   void editSkelKeyframeTime(unsigned long index, double newTime);
   void editCageKeyframeTime(unsigned long index, double newTime);

   void setAnimationSpeed(double s);

   inline auto getNumberOfSkelKeyframes() const {return skelKeyframeTime.size();}
   inline auto getNumberOfCageKeyframes() const {return cageKeyframeTime.size();}
   inline auto getSkelKeyframeIndex() const {return playingSkelKeyframeIndex;}
   inline auto getCageKeyframeIndex() const {return playingCageKeyframeIndex;}
   const std::vector<double> & getSkelKeyframeTimeVector() const;
   const std::vector<double> & getCageKeyframeTimeVector() const;
   const std::vector<std::vector<cg3::Transform>> & getSkelKeyframeVector() const;
   const std::vector<std::vector<double> > & getCageKeyframeVector() const;


   void loadSkelAnimation(const std::vector<double> &_tSkel,
                          const std::vector<std::vector<cg3::Transform> > &_skelKeyframes);
   void loadCageAnimation(const std::vector<double> &_tCage,
                          const std::vector<std::vector<double> > &_cageKeyframes);
private:
   std::vector<double> cageKeyframeTime;
   std::vector<double> skelKeyframeTime;
   std::vector<std::vector<double>> cageKeyframes;
   std::vector<std::vector<cg3::Transform>> skelKeyframes;  //just rotations!

   Cage * cage;
   Skeleton * skel;

   QTime tSkel;
   QTime tCage;

   int playingSkelKeyframeIndex;
   int playingCageKeyframeIndex;

   double animationSpeed;
};

#endif // ASYNCANIMATOR_H
