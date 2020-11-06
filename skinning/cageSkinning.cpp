#include "cageSkinning.h"

CageSkinning::CageSkinning()
{

}

CageSkinning::CageSkinning(Character *_character,
                           Cage *_cage)
{
    create(_character, _cage);
}

bool CageSkinning::create(Character *_character, Cage *_cage)
{
    clear();

    setCharacter(_character);
    setCage(_cage);

    return true;
}

void CageSkinning::clear()
{
    character = nullptr;
    cage = nullptr;
}
