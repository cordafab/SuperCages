#include "characterOperations.h"

#include <vector>

#include "GUI/qtUtils.h"
#include "common/importFiles.h"
#include "common/exportFiles.h"
#include "controller.h"
#include "drawables/drawableCharacter.h"
#include "skinning/cageSkinning.h"
#include "GUI/glCanvas.h"

void clearCharacter()
{
   Controller * c = Controller::get();

   c->isCharacterLoaded = false;
   c->glCanvas->removeDrawableObject(c->character);
   c->character->clear();

   updateGUI();
}


void saveCharacterToFile()
{
   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save Character", "3D Meshes (*.obj *.ply)"))
   {
      if(c->isCharacterLoaded)
      {
         std::vector<double> v = c->character->getVerticesVector();
         std::vector<int>    f = c->character->getTrianglesVector();

         saveMesh(filename.c_str(), v, f);
      }
   }

}
