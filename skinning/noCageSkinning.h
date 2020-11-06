#ifndef NOCAGESKINNING_H
#define NOCAGESKINNING_H

#include "skinning/cageSkinning.h"


class NoCageSkinning:
      public CageSkinning
{
public:
   NoCageSkinning();
   NoCageSkinning(Character   * _character,
                  Cage        * _cage);

   bool create (Character   * _character,
                Cage        * _cage);

   void clear();

   void deform();
   Weights * getWeights();

private:

   Weights weights;
};

#endif // NOCAGESKINNING_H
