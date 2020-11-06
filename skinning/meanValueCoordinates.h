#ifndef MEANVALUECOORDINATES_H
#define MEANVALUECOORDINATES_H

#include "skinning/weights.h"
#include "rigs/character.h"
#include "rigs/cage.h"
#include "geom/vec3.h"
#include "skinning/cageSkinning.h"

class MeanValueCoordinates:
      public CageSkinning
{
public:
   MeanValueCoordinates();
   MeanValueCoordinates(Weights     * _w,
                        Character   * _character,
                        Cage        * _cage);
   MeanValueCoordinates(Character   * _character,
                        Cage        * _cage);

   ~MeanValueCoordinates();


   bool create         (Weights     * _w,
                        Character   * _character,
                        Cage        * _cage);
   bool create         (Character   * _character,
                        Cage        * _cage);

   void clear();

   void deform();

   static bool generateCoords(Weights           * & weights,
                              Character         *   character,
                              Cage              *   cage);
   Weights * getWeights();

private:

   Weights * weights;

};


#endif // MEANVALUECOORDINATES_H
