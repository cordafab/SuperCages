#ifndef DRAWABLESKELETON_H
#define DRAWABLESKELETON_H

#include <vector>
#include <unordered_set>
#include "rigs/skeleton.h"
#include "drawables/drawableObject.h"
#include "drawables/pickableObject.h"
#include "geom/boundingBox.h"
#include "drawables/pickerController.h"

class DrawableSkeleton
      : public Skeleton,
        public DrawableObject,
        public PickableObject
{

public:

    enum
    {
        DRAW_SKELETON     = 0x00000001,
        DRAW_JOINTS       = 0x00000002,
        DRAW_BONES        = 0x00000004,
    };

    DrawableSkeleton();
    DrawableSkeleton(const std::vector<cg3::Vec3d>     & joints,
                     const std::vector<cg3::Vec3d>     & jointsRotations,
                     const std::vector<int>            & fathers,
                     const std::vector<std::string>    & names);

    bool create      ();

    bool create      (const std::vector<cg3::Vec3d>    & joints,
                      const std::vector<cg3::Vec3d>    & jointsRotations,
                      const std::vector<int>           & fathers,
                      const std::vector<std::string>   & names);

    ~DrawableSkeleton();

    void init();
    void clear();

    //DrawableObject interface
    void draw() const;
    void drawRest() const;
    void drawWithNames();
    bool sceneCenter(cg3::Vec3d & center) const;
    bool sceneRadius(double & radius) const;

    //skinning flags
    inline void refreshCharacterPose() { _refreshCharacterPose = true;}
    inline bool refreshCharacterPoseIsNeeded() const { return _refreshCharacterPose; }
    inline void characterPoseRefreshed() { _refreshCharacterPose = false; }

    //technical stuff for picking
    void initPicking();

    //PickableObject interface
    void selectObject(const unsigned long pickableIndex);
    void deselectObject(const unsigned long pickableIndex);

    //Deformation
    void translate(const cg3::Vec3d & translation);
    void rotate(const cg3::dQuaternion & rotation);

    //Deformation Rest Pose
    //void translateRest(const cg3::Vec3d & translation);
    //void rotateRest(const cg3::dQuaternion & rotation);

    inline const std::vector<bool> & getSelectedNodes() const
    { return sonsOfSelectedNode; }

    inline int getSelectedFatherNode() const
    { return selectedFatherNode; }

    void activateSkeletonRendering(bool activate);

    bool isRootMotionEnabled;

protected:

    int drawMode;
    bool _refreshCharacterPose;

    //technical stuff for picking
    PickerController*         pickerController;
    std::map<int,int>         pickableIndex2Node;
    std::map<int,int>         node2PickableIndex;
    int                       selectedFatherNode;
    std::vector<bool>         sonsOfSelectedNode;
    std::unordered_set<int>   selectedNodes;

    void findSonsOfNode(int nodeIndex);

};

#endif // DRAWABLESKELETON_H
