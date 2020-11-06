#ifndef CAGESKINNING_H
#define CAGESKINNING_H

#include "skinning/weights.h"
#include "rigs/cage.h"
#include "rigs/character.h"
#include "rigs/skeleton.h"
#include "geom/transform.h"

class CageSkinning
{
public:
   CageSkinning();
   CageSkinning(Character * _character,
                Cage * _cage);

   bool create(Character * _character,
               Cage * _cage);

   void clear();

   inline void setCharacter(Character * _character) { character = _character; }
   inline void setCage(Cage * _cage) { cage = _cage; }

   virtual void deform() = 0;
   virtual Weights * getWeights() = 0;

protected:

   Character * character;
   Cage * cage;
};

#endif // CAGESKINNING_H
