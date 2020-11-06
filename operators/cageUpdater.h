#ifndef CAGEUPDATER_H
#define CAGEUPDATER_H

#include "skinning/weights.h"
#include "rigs/cage.h"
#include "rigs/character.h"
#include "geom/transform.h"
#include "math/leastSquareSolver.h"

class CageUpdater
{
public:
   CageUpdater();
   CageUpdater(Weights * _w,
               Character * _character,
               Cage * _cage);
   CageUpdater(Weights * _w,
               Character * _character,
               Cage * _cage,
               const std::vector<int> & _selectedVertices);

   bool create(Weights * _w,
               Character * _character,
               Cage * _cage);
   bool create(Weights * _w,
               Character * _character,
               Cage * _cage,
               const std::vector<int> & _selectedVertices);

   void clear();

   //implement destructor

   void updatePosition();

private:

   inline void setWeights(Weights * _w) { w = _w; }
   inline void setCharacter(Character * _character) { character = _character; }
   inline void setCage(Cage * _cage) { cage = _cage; }

protected:
   Weights * w;
   Character * character;
   Cage * cage;

   std::vector< int > selectedVertices;

   //Solver
   LeastSquareSolver solver;
};

#endif // CAGEUPDATER_H
