#include "drawableSkeleton.h"

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

#include "GUI/glUtils.h"
#include <stack>

DrawableSkeleton::DrawableSkeleton()
{
    create();
}

DrawableSkeleton::DrawableSkeleton(const std::vector<cg3::Vec3d>     & joints,
                                   const std::vector<cg3::Vec3d> & jointsRotations,
                                   const std::vector<int>            & fathers,
                                   const std::vector<std::string>    & names)

{
    create(joints, jointsRotations, fathers, names);
}

bool DrawableSkeleton::create()
{
    clear();

    init();

    return true;
}

bool DrawableSkeleton::create(const std::vector<cg3::Vec3d> &joints, const std::vector<cg3::Vec3d> &jointsRotations, const std::vector<int> &fathers, const std::vector<std::string> &names)
{
    clear();

    Skeleton::create(joints, jointsRotations, fathers, names);

    init();
    //TODO: call initPicking() here

    return true;
}


DrawableSkeleton::~DrawableSkeleton()
{
    clear();
}

void DrawableSkeleton::init()
{
    type           = SKELETON;
    drawMode       = DRAW_SKELETON | DRAW_JOINTS | DRAW_BONES;

    pickerController = PickerController::get();
}

void DrawableSkeleton::clear()
{
   Skeleton::clear();

   if(pickerController)
   {
      for(ulong i=0; i<getNumNodes(); ++i)
      {
         pickerController->removeIndex(node2PickableIndex[i]);
      }
   }

   sonsOfSelectedNode.clear();
   selectedFatherNode = -1;
   _refreshCharacterPose = false;
   pickerController = nullptr;
   drawMode = 0;

   pickableIndex2Node.clear();
   node2PickableIndex.clear();
   selectedNodes.clear();
}

void DrawableSkeleton::draw() const
{
    if (drawMode & DRAW_SKELETON)
    {
        glDisable(GL_DEPTH_TEST);

        if(drawMode & DRAW_BONES)
        {
            glDisable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            setSingleLighting();
            for(ulong i=0; i<getNumNodes(); ++i)
            {
                SkeletonNode n = getNode(i);
                cg3::Vec3d p = n.getGlobalTCurrent().getTranslation();
                for(int sonIndex: n.getNext())
                {
                    SkeletonNode son = getNode(sonIndex);
                    cg3::Vec3d s = son.getGlobalTCurrent().getTranslation();
                    glLineWidth(4.0);
                    glColor4f(0.156, 0.333, 0.8, 0.7);
                    glBegin(GL_LINES);
                    glVertex3f(p.x(), p.y(), p.z());
                    glVertex3f(s.x(), s.y(), s.z());
                    glEnd();
                }
            }
        }

        if(drawMode & DRAW_JOINTS)
        {
            glEnable(GL_LIGHTING);
            glShadeModel(GL_FLAT);
            setSingleLighting();
            double radius = boundingBox.diagonal()/150.0;
            if (radius == 0) radius = 0.1;

            for(ulong i=0; i<getNumNodes(); ++i)
            {
                SkeletonNode n = getNode(i);
                cg3::Vec3d p = n.getGlobalTCurrent().getTranslation();
                if(sonsOfSelectedNode[i])
                {
                    drawSphere(p, radius, 0.905, 0.305, 0.305);
                }
                else
                {
                    drawSphere(p, radius, 0.156, 0.333, 0.553);
                }
            }
        }
        glEnable(GL_DEPTH_TEST);
    }
}

void DrawableSkeleton::drawRest() const
{
   if (drawMode & DRAW_SKELETON)
   {
      glDisable(GL_DEPTH_TEST);

      if(drawMode & DRAW_BONES)
      {
         glDisable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         for(ulong i=0; i<getNumNodes(); ++i)
         {
            SkeletonNode n = getNode(i);
            cg3::Vec3d p = n.getGlobalTRest().getTranslation();
            for(int sonIndex: n.getNext())
            {
               SkeletonNode son = getNode(sonIndex);
               cg3::Vec3d s = son.getGlobalTRest().getTranslation();
               glLineWidth(4.0);
               glColor4f(0.156, 0.333, 0.8, 0.7);
               glBegin(GL_LINES);
               glVertex3f(p.x(), p.y(), p.z());
               glVertex3f(s.x(), s.y(), s.z());
               glEnd();
            }
         }

      }

      if(drawMode & DRAW_JOINTS)
      {
         glEnable(GL_LIGHTING);
         glShadeModel(GL_FLAT);
         setSingleLighting();
         double radius = boundingBox.diagonal()/150.0;
         if (radius == 0) radius = 0.1;

         for(ulong i=0; i<getNumNodes(); ++i)
         {
            SkeletonNode n = getNode(i);
            cg3::Vec3d p = n.getGlobalTRest().getTranslation();

            drawSphere(p, radius, 0.156, 0.333, 0.553);

         }
      }
      glEnable(GL_DEPTH_TEST);
   }
}

void DrawableSkeleton::drawWithNames()
{
    if (drawMode & DRAW_SKELETON)
    {
        if(drawMode & DRAW_JOINTS)
        {
            double radius = boundingBox.diagonal()/100.0;
            if (radius == 0) radius = 0.1;

            for(ulong i=0; i<getNumNodes(); ++i)
            {
                glPushMatrix();
                glPushName(node2PickableIndex[i]);
                SkeletonNode n = getNode(i);
                cg3::Vec3d p = n.getGlobalTCurrent().getTranslation();
                drawDiamond(p, radius, 0.0, 0.0, 1.0, true);
                glPopName();
                glPopMatrix();
            }
        }
    }
}

bool DrawableSkeleton::sceneCenter(cg3::Vec3d & center) const
{
    center = boundingBox.center();
    return true;   //center is a valid value
}

bool DrawableSkeleton::sceneRadius(double & radius) const
{
    radius = boundingBox.diagonal();
    return true;   //center is a valid value
}


void DrawableSkeleton::selectObject(const unsigned long pickableIndex)
{
    std::fill(sonsOfSelectedNode.begin(), sonsOfSelectedNode.end(), false);
    selectedNodes.clear();
    selectedFatherNode = pickableIndex2Node[pickableIndex];
    selectedNodes.insert(selectedFatherNode);
    findSonsOfNode(selectedFatherNode);
}

void DrawableSkeleton::deselectObject(const unsigned long pickableIndex)
{
    selectedNodes.clear();
    if(selectedFatherNode == pickableIndex2Node[pickableIndex])
    {
        std::fill(sonsOfSelectedNode.begin(), sonsOfSelectedNode.end(), false);
        selectedFatherNode = -1;
    }
}

void DrawableSkeleton::translate(const cg3::Vec3d & translation)
{
    /*if(selectedFatherNode != -1 && activateTransformation)
   {
      cg3::Transform pose(translation.x(), translation.y(), translation.z());
      addGlobalTransformation(selectedFatherNode, pose);
      _refreshCharacterPose = true;
   }*/
}

void DrawableSkeleton::rotate(const cg3::dQuaternion & rotation)
{
    if(selectedFatherNode != -1 && activateTransformation)
    {
        cg3::Transform pose(rotation);
        addGlobalTransformation(selectedFatherNode, pose); //BUGGY
        _refreshCharacterPose = true;
        updateGlobalT();
    }
}

/*void DrawableSkeleton::translateRest(const cg3::Vec3d & translation)
{
    if(selectedFatherNode != -1 && activateTransformation)
    {
        cg3::Transform pose(translation.x(), translation.y(), translation.z());
        //applyRestPose(selectedFatherNode, pose);
        _refreshCharacterPose = true;
        updateGlobalT();
    }
}*/

/*void DrawableSkeleton::rotateRest(const cg3::dQuaternion & rotation)
{
    if(selectedFatherNode != -1 && activateTransformation)
   {
      cg3::Transform pose(rotation);
      applyRestPose(selectedFatherNode, pose);
      _refreshCharacterPose = true;
   }
}*/

void DrawableSkeleton::initPicking()
{
    //assign PickableIndexes to every vertex
    for(ulong i=0; i<getNumNodes(); ++i)
    {
        int pickableIndex = pickerController->generateIndex(this);
        pickableIndex2Node[pickableIndex] = i;
        node2PickableIndex[i] = pickableIndex;
    }

    //initialize sonsOfSelectedNodes vector with false (all deselected)
    sonsOfSelectedNode.resize(getNumNodes(), false);
}

void DrawableSkeleton::activateSkeletonRendering(bool activate)
{
    if (activate)  drawMode |=  DRAW_SKELETON;
    else           drawMode &= ~DRAW_SKELETON;
}

void DrawableSkeleton::findSonsOfNode(int nodeIndex)
{
    sonsOfSelectedNode[nodeIndex] = true;
    selectedNodes.insert(nodeIndex);
    for(int n: nodes[nodeIndex].getNext())
        findSonsOfNode(n);
}
