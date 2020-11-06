#include "clickConverter.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

ClickConverter::ClickConverter()
{

}

void ClickConverter::init(const int mouseX,
                          const int mouseY,
                          const cg3::Point3d & cameraPosition,
                          const cg3::Vec3d & cameraDirection,
                          const cg3::Point3d & sceneCenter,
                          const float projectionMatrix[],
                          const float viewMatrix[],
                          const int windowsWidth,
                          const int windowsHeight)
{
   _previousMouseX = mouseX;
   _previousMouseY = mouseY;
   _actualMouseX = mouseX;
   _actualMouseY = mouseY;
   _cameraPosition = cameraPosition;
   _cameraDirection = cameraDirection;

   for(int i=0; i<16; ++i)
   {
      _projectionMatrix[i] = projectionMatrix[i];
      _viewMatrix[i] = viewMatrix[i];
   }

   _windowsWidth = windowsWidth;
   _windowsHeight = windowsHeight;

   midPlane.set(sceneCenter,
                _cameraDirection);

   generateRay(mouseX, mouseY, previousClickRay);
   generateRay(mouseX, mouseY, actualClickRay);
}

void ClickConverter::updateMouseMovement(const int mouseX, const int mouseY)
{
   _actualMouseX = mouseX;
   _actualMouseY = mouseY;
   generateRay(mouseX, mouseY, actualClickRay);
}

void ClickConverter::getTranslation(cg3::Vec3d & translation)
{
   cg3::Vec3d vPreviousRay = previousClickRay.getPointOnRay(50.0);
   cg3::Vec3d vActualRay = actualClickRay.getPointOnRay(50.0);

   cg3::Vec3d p1;
   midPlane.computeIntersectionWithLine(_cameraPosition, vPreviousRay, p1);

   cg3::Vec3d p2;
   midPlane.computeIntersectionWithLine(_cameraPosition, vActualRay, p2);

   translation = p2-p1;

   _previousMouseX = _actualMouseX;
   _previousMouseY = _actualMouseY;
   previousClickRay = actualClickRay;
}

void ClickConverter::getRotation(cg3::dQuaternion & rotation,
                                 const double scaleFactor)
{
   cg3::Vec3d vPreviousRay = previousClickRay.getPointOnRay(50.0);
   cg3::Vec3d vActualRay = actualClickRay.getPointOnRay(50.0);

   cg3::Vec3d p1;
   midPlane.computeIntersectionWithLine(_cameraPosition, vPreviousRay, p1);

   cg3::Vec3d p2;
   midPlane.computeIntersectionWithLine(_cameraPosition, vActualRay, p2);

   cg3::Vec3d delta = p2-p1;

   //Utilities for rotations
   cg3::Vec3d rotationAxis = delta.cross(midPlane.normal());

   /*//TODO: find a better and more precise way to compute the rotation angle
   double clicksDistance = std::sqrt(((_previousMouseX - _actualMouseX)  *
                                      (_previousMouseX - _actualMouseX)) +
                                     ((_previousMouseY - _actualMouseY)  *
                                      (_previousMouseY - _actualMouseY)));
   double angle = std::abs(clicksDistance/15.0);*/

   p1.normalize();
   p2.normalize();
   //double angle = acos(p1.dot(p2));
   double angle = p1.dot(p2);

   rotation.setRotation(rotationAxis,
                        angle/(scaleFactor/3));

   _previousMouseX = _actualMouseX;
   _previousMouseY = _actualMouseY;
   previousClickRay = actualClickRay;
}


//Generates a ray from the mouse clickes a ray from the mouse click
void ClickConverter::generateRay(const int mouseX, const int mouseY, cg3::dRay & ray) const
{
   //this function runs in 12microseconds. Libqgl's runs in 5.
   //Normalized Device Space
   float xNds = (2.0f * (float) mouseX) / (float) _windowsWidth - 1.0f;
   float yNds = 1.0f - (2.0f * (float) mouseY) / (float) _windowsHeight;
   float zNds = 1.0f;
   glm::vec3 rayNds(xNds, yNds, zNds);

   //Homogeneous Clip Space
   glm::vec4 rayHcs(rayNds.x, rayNds.y, -1.0, 1.0);

   //Eye Space
   glm::mat4x4 projMatrix = glm::make_mat4(_projectionMatrix);
   glm::vec4 rayEye = glm::inverse(projMatrix) * rayHcs;
   rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0);

   //World Space
   glm::mat4x4 viewMatrix = glm::make_mat4(_viewMatrix);
   glm::vec4 rayWorldGlm = glm::inverse(viewMatrix) * rayEye;
   cg3::Vec3d rayWorld(rayWorldGlm.x, rayWorldGlm.y, rayWorldGlm.z);
   rayWorld.normalize();

   ray = cg3::dRay(_cameraPosition, rayWorld);
}
