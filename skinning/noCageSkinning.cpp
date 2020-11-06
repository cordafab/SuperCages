#include "noCageSkinning.h"

NoCageSkinning::NoCageSkinning()
{

}

NoCageSkinning::NoCageSkinning(Character * _character, Cage * _cage)
{
    create(_character,_cage);
}

bool NoCageSkinning::create(Character *_character, Cage *_cage)
{
    clear();

    CageSkinning::create(_character, _cage);

    weights = Weights(0,0);

    return true;
}

void NoCageSkinning::clear()
{
    CageSkinning::clear();
}

void NoCageSkinning::deform()
{

}

Weights * NoCageSkinning::getWeights()
{
   return & weights;
}
