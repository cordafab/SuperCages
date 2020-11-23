#include "asyncAnimator.h"

AsyncAnimator::AsyncAnimator()
{

}

AsyncAnimator::AsyncAnimator(Cage *_cage, Skeleton *_skel)
{
    create(_cage, _skel);
}

bool AsyncAnimator::create(Cage *_cage, Skeleton *_skel)
{
    clear();

    cage = _cage;
    skel = _skel;

    playingCageKeyframeIndex = -1;
    playingSkelKeyframeIndex = -1;

    return true;
}

void AsyncAnimator::clear()
{
    cageKeyframeTime.clear();
    skelKeyframeTime.clear();
    cageKeyframes.clear();
    skelKeyframes.clear();

    cage = nullptr;
    skel = nullptr;

    //tSkel.restart();
    //tCage.restart();

    playingSkelKeyframeIndex = -1;
    playingCageKeyframeIndex = -1;

    animationSpeed = 1.0;
}

void AsyncAnimator::loadAnimation(
      const std::vector<double> &_tCage,
      const std::vector<double> &_tSkel,
      const std::vector<std::vector<double> > &_cageKeyframes,
      const std::vector<std::vector<cg3::Transform> > &_skelKeyframes
      )
{
   cageKeyframeTime = _tCage;
   skelKeyframeTime = _tSkel;
   cageKeyframes    = _cageKeyframes;
   skelKeyframes    = _skelKeyframes;
}

void AsyncAnimator::loadSkelAnimation(
      const std::vector<double> &_tSkel,
      const std::vector<std::vector<cg3::Transform> > &_skelKeyframes
      )
{
   skelKeyframeTime = _tSkel;
   skelKeyframes    = _skelKeyframes;
}

void AsyncAnimator::loadCageAnimation(
      const std::vector<double> &_tCage,
      const std::vector<std::vector<double> > &_cageKeyframes
      )
{
   cageKeyframeTime = _tCage;
   cageKeyframes    = _cageKeyframes;
}

void  AsyncAnimator::addCageKeyframe(
                            double t,
                            const std::vector<double> & cageKeyframe)
{
   ulong i=0;

   //std::vector<double>::iterator it = keyframeTime.begin();
   while(i<cageKeyframeTime.size() && cageKeyframeTime[i] < t )
   {
      i++;
   }

   if(i==cageKeyframeTime.size())
   {
      cageKeyframeTime.push_back(t);
      cageKeyframes.push_back(cageKeyframe);
   } else {
      cageKeyframeTime.insert(cageKeyframeTime.begin()+i,t);
      cageKeyframes.insert(cageKeyframes.begin()+i,cageKeyframe);
   }
}

void  AsyncAnimator::addSkelKeyframe(
                            double t,
                            const std::vector<cg3::Transform> & skelKeyframe)
{
   ulong i=0;

   //std::vector<double>::iterator it = keyframeTime.begin();
   while(i<skelKeyframeTime.size() && skelKeyframeTime[i] < t )
   {
      i++;
   }

   if(i==skelKeyframeTime.size())
   {
      skelKeyframeTime.push_back(t);
      skelKeyframes.push_back(skelKeyframe);
   } else {
      skelKeyframeTime.insert(skelKeyframeTime.begin()+i,t);
      skelKeyframes.insert(skelKeyframes.begin()+i,skelKeyframe);
   }
}

void AsyncAnimator::rewindAnimator()
{
   playingCageKeyframeIndex = -1;
   playingSkelKeyframeIndex = -1;
}

void AsyncAnimator::updateFrame()
{
   if((playingCageKeyframeIndex == -1) &&
      (playingSkelKeyframeIndex == -1)
     )
   {
      tSkel.start();
      tCage.start();
      playingCageKeyframeIndex = 0;
      playingSkelKeyframeIndex = 0;
   }
   if(playingSkelKeyframeIndex == skelKeyframeTime.size())

   {
      tSkel.restart();

      playingSkelKeyframeIndex = 0;
   }
   if(playingCageKeyframeIndex == cageKeyframeTime.size())
   {
      tCage.restart();
      playingCageKeyframeIndex = 0;
      playingSkelKeyframeIndex = 0;
   }

   double speed = animationSpeed * 1000.0;

   setSkeletonFrame(((double)tSkel.elapsed())/speed);
   setCageFrame(((double)tCage.elapsed())/speed);
}

void AsyncAnimator::setSkeletonFrame(double t)
{

   while((playingSkelKeyframeIndex<skelKeyframeTime.size()) &&
         (skelKeyframeTime[playingSkelKeyframeIndex]<t))
   { playingSkelKeyframeIndex++; }

   if(skelKeyframeTime.size()>0)
   {
      if(playingSkelKeyframeIndex==skelKeyframeTime.size()) {
         skel->setKeyframe(skelKeyframes[playingSkelKeyframeIndex-1]);
      } else if (playingSkelKeyframeIndex==0) {
         skel->setKeyframe(skelKeyframes[0]);
      } else {
         double low = skelKeyframeTime[playingSkelKeyframeIndex-1];
         double top = skelKeyframeTime[playingSkelKeyframeIndex];
         double a = (t-low) / (top - low);
         skel->interpolateKeyframes(skelKeyframes[playingSkelKeyframeIndex-1],
                                    skelKeyframes[playingSkelKeyframeIndex],
                                    a);
      }
   }
}

void AsyncAnimator::setCageFrame(double t)
{
   while((playingCageKeyframeIndex<cageKeyframeTime.size()) &&
         (cageKeyframeTime[playingCageKeyframeIndex]<t))
   { playingCageKeyframeIndex++; }


   if(cageKeyframeTime.size()>0)
   {
      if(playingCageKeyframeIndex==cageKeyframeTime.size()) {

         cage->setKeyframe(cageKeyframes[playingCageKeyframeIndex-1]);

      } else if (playingCageKeyframeIndex==0) {
         cage->setKeyframe(cageKeyframes[0]);
      } else {
         double low = cageKeyframeTime[playingCageKeyframeIndex-1];
         double top = cageKeyframeTime[playingCageKeyframeIndex];
         double a = (t-low) / (top - low);
         cage->interpolateKeyframes(
                  cageKeyframes[playingCageKeyframeIndex-1],
               cageKeyframes[playingCageKeyframeIndex],
               a);
      }
   }
}

void AsyncAnimator::setSkeletonKeyframeIndex(unsigned long frameIndex)
{
   if(frameIndex < skelKeyframeTime.size())
   {
      skel->setKeyframe(skelKeyframes[frameIndex]);
      playingSkelKeyframeIndex = frameIndex;
   }
}

void AsyncAnimator::setCageKeyframeIndex(unsigned long frameIndex)
{
   if(frameIndex < cageKeyframeTime.size())
   {
      cage->setKeyframe(cageKeyframes[frameIndex]);
      playingCageKeyframeIndex = frameIndex;
   }
}

void AsyncAnimator::deleteSkelKeyframe(unsigned long frameIndex)
{
   if(frameIndex < skelKeyframes.size())
   {
      skelKeyframeTime.erase(skelKeyframeTime.begin()+frameIndex);
      skelKeyframes.erase(skelKeyframes.begin()+frameIndex);
   }
}

void AsyncAnimator::deleteCageKeyframe(unsigned long frameIndex)
{
   if(frameIndex < cageKeyframes.size())
   {
      cageKeyframeTime.erase(cageKeyframeTime.begin()+frameIndex);
      cageKeyframes.erase(cageKeyframes.begin()+frameIndex);
   }
}

void AsyncAnimator::editSkelKeyframeTime(unsigned long index, double newTime)
{
   std::vector<cg3::Transform> skelKeyframe = skelKeyframes[index];
   deleteSkelKeyframe(index);
   addSkelKeyframe(newTime, skelKeyframe);
}

void AsyncAnimator::editCageKeyframeTime(unsigned long index, double newTime)
{
   std::vector<double> cageKeyframe = cageKeyframes[index];
   deleteCageKeyframe(index);
   addCageKeyframe(newTime, cageKeyframe);
}

void AsyncAnimator::setAnimationSpeed(double s)
{
   animationSpeed = s;
}

const std::vector<double> &AsyncAnimator::getSkelKeyframeTimeVector() const
{
   return skelKeyframeTime;
}

const std::vector<double> &AsyncAnimator::getCageKeyframeTimeVector() const
{
   return cageKeyframeTime;
}

const std::vector<std::vector<cg3::Transform>> & AsyncAnimator::getSkelKeyframeVector() const
{
   return skelKeyframes;
}

const std::vector<std::vector<double>> & AsyncAnimator::getCageKeyframeVector() const
{
   return cageKeyframes;
}
