#include "skeletonSkinning.h"

SkeletonSkinning::SkeletonSkinning()
{

}

SkeletonSkinning::SkeletonSkinning(Weights   *_w,
                                   Character *_character,
                                   Skeleton  *_skeleton)
{
    create(_w, _character, _skeleton);
}

bool SkeletonSkinning::create(Weights *_w,
                              Character *_character,
                              Skeleton *_skeleton)
{
    clear();

    setWeights(_w);
    setCharacter(_character);
    setSkeleton(_skeleton);

    return true;
}

void SkeletonSkinning::clear()
{
    w = nullptr;
    character = nullptr;
    skeleton = nullptr;
}


