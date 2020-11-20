#include "rigOperations.h"


#include "controller.h"

#include "GUI/qtUtils.h"
#include "GUI/glCanvas.h"
#include "GUI/restPoseCanvas.h"

#include "common/importFiles.h"
#include "common/exportFiles.h"
#include "common/characterOperations.h"
#include "common/skeletonOperations.h"
#include "common/cageOperations.h"
#include "common/animatorOperations.h"

#include "drawables/drawableCharacter.h"
#include "drawables/drawableCage.h"
#include "drawables/drawableSkeleton.h"

#include "skinning/cageSkinning.h"
#include "skinning/noCageSkinning.h"
#include "skinning/meanValueCoordinates.h"
#include "skinning/cageSkinning.h"
#include "operators/cageUpdater.h"
#include "skinning/linearBlendSkinning.h"
#include "skinning/dualQuaternionSkinning.h"
#include "operators/skeletonUpdater.h"
#include "skinning/corSkinning.h"

#include "skinning/sparseWeights.h"

#include <iostream>
#include <fstream>


#include "cinolib/io/read_OBJ.h"

#include <QElapsedTimer>


void importRig()
{

   //QElapsedTimer timer;
   //qint64 nanoSec;
   //timer.start();

   std::string filename;
   Controller * c = Controller::get();

   clearRig();

   //   filename = "/media/jmt/DATA/Work/Models/warrok/warrok.obj";
   if (openFileSelectionDialog(filename, "Load Rig", "3D Meshes (*.obj *.ply)",(QWidget*)c->mainWindow))
   {

      std::string extension = filename.substr(filename.size()-4,4);
      std::string filenameNoExt = filename.substr(0, filename.size()-4);
      std::cout << "Loading Rig File: " << filenameNoExt << std::endl;

      importCharacter(filename);
      importSkeleton(filenameNoExt+".skt");
      importSkeletonWeights(filenameNoExt+".skw");
      importCage("none");
      importCageWeights("none");
      importCoR(filenameNoExt+"_corWeights.txt");
      importMAXVOL(filenameNoExt+"_maxvol.txt");


      //std::cout << "!! Start Skeleton Updater " << std::endl;
      //timer.restart();
      initializeSkeletonUpdater();
      //nanoSec = timer.elapsed();
      //std::cout << "!! initializeSkeletonUpdater computed in " << nanoSec << " mSec." << std::endl;

      //std::cout << "!! Start Cage Updater " << std::endl;
      //timer.restart();
      initializeCageUpdater();
      //nanoSec = timer.elapsed();
      //std::cout << "!! initializeCageUpdater computed in " << nanoSec << " mSec." << std::endl;

      //std::cout << "!! Start init animator " << std::endl;
      //timer.restart();
      initializeAnimator();
      //nanoSec = timer.elapsed();
      //std::cout << "!! initializeAnimator computed in " << nanoSec << " mSec." << std::endl;
   }

   updateGUI();

}

void exportRig()
{
   std::string filename;
   Controller * c = Controller::get();

   if (openFileSaveDialog(filename, "Save Character", "3D Meshes (*.obj *.ply)"))
   {

      std::string extension = filename.substr(filename.size()-4,4);
      std::string filenameNoExt = filename.substr(0, filename.size()-4);

      if(c->isCharacterLoaded)
      {
         const std::vector<double> & v = c->character->getVerticesVector();
         const std::vector<int>    & f = c->character->getTrianglesVector();

         saveMesh(filename.c_str(), v, f);
      }

      if(c->isCageLoaded)
      {
         std::vector<double> v = c->cage->getVerticesVector();
         std::vector<int>    f = c->cage->getTrianglesVector();

         saveMesh((filenameNoExt+"_cage"+extension).c_str(), v, f);
      }

      if(c->areCageWeightsLoaded)
      {
         saveWeights((filenameNoExt+"_cageWeights.txt").c_str(), c->cageWeights);
      }

      if(c->isSkeletonLoaded)
      {
         std::vector<Node> nodes = c->skeleton->getNodesVector();
         std::vector<cg3::Vec3d> joints;
         std::vector<int> fathers;
         std::vector<std::string> names;

         for(int i=0; i < c->skeleton->getNumNodes(); ++i)
         {
            joints.push_back(nodes[i].getGlobalTRest().getTranslation());
            fathers.push_back(nodes[i].getFather());
            names.push_back(nodes[i].getNodeName());
         }

         saveSkeleton((filenameNoExt+"_skel.txt").c_str(),
                      joints,
                      fathers,
                      names);

         if(c->areSkeletonWeightsLoaded)
         {
            saveWeights((filenameNoExt+"_skelWeights.txt").c_str(), c->skeletonWeights);
         }
      }
   }
}

void clearRig()
{
   clearAnimator();
   clearSkeleton();
   clearCage();
   clearCharacter();

   updateGUI();
}



void importCharacter(const std::string & filename)
{
   Controller * c = Controller::get();

   //"cut" mesh
   std::vector<cinolib::vec3d> xyz;
   std::vector<cinolib::vec3d> uvw;
   std::vector<std::vector<uint>> poly;

   //original mesh
   std::vector<cinolib::vec3d> pos, tex, nor;
   std::vector<std::vector<uint>> poly_pos, poly_tex, poly_nor;
   std::vector<cinolib::Color> poly_col;
   std::string  diffuse_path;
   std::string  specular_path;
   std::string  normal_path;
   cinolib::read_OBJ(filename.c_str(), pos, tex, nor, poly_pos, poly_tex, poly_nor, poly_col, diffuse_path, specular_path, normal_path);

   if (poly_pos.size() == poly_tex.size())
   {
      cinolib::cut_mesh_along_seams(pos, tex, poly_pos, poly_tex, xyz, uvw, poly);
   }

   std::vector<double> v;
   std::vector<int> f;

   std::vector<double> vCut;
   std::vector<float> uv;
   std::vector<int> fCut;

   for(int i=0; i<pos.size(); ++i)
   {
      v.push_back(pos[i].x());
      v.push_back(pos[i].y());
      v.push_back(pos[i].z());
   }

   for(int i=0; i<poly_pos.size(); ++i)
   {
      if(poly_pos[i].size()==3)
      {
         f.push_back(poly_pos[i][0]);
         f.push_back(poly_pos[i][1]);
         f.push_back(poly_pos[i][2]);
      }
      else if(poly_pos[i].size()==4)
      {
         //TO DO: Improve quad-to-tris algorithm
         f.push_back(poly_pos[i][0]);
         f.push_back(poly_pos[i][1]);
         f.push_back(poly_pos[i][2]);

         f.push_back(poly_pos[i][0]);
         f.push_back(poly_pos[i][2]);
         f.push_back(poly_pos[i][3]);
      }
   }

   for(int i=0; i<xyz.size(); ++i)
   {
      vCut.push_back(xyz[i].x());
      vCut.push_back(xyz[i].y());
      vCut.push_back(xyz[i].z());
   }

   for(int i=0; i<poly.size(); ++i)
   {
      fCut.push_back(poly[i][0]);
      fCut.push_back(poly[i][1]);
      fCut.push_back(poly[i][2]);
   }

   for(unsigned long i=0; i<uvw.size(); ++i)
   {
      uv.push_back(uvw[i].x());
      uv.push_back(uvw[i].y());
   }

   c->character->create(v, f, vCut, uv, fCut);

   if(!diffuse_path.empty())
   {
      c->isDiffuseTextureInitialized = true;
      c->character->diffuse_tex = diffuse_path;
      //c->character->updateTexture2D(1.0, diffuse_path.c_str());
   }

   c->glCanvas->pushDrawableObject(c->character);
   c->isCharacterLoaded = true;
}

void importSkeleton(const std::string & filename)
{
   Controller * c = Controller::get();

   //Load Character Skeleton
   std::vector<cg3::Vec3d>     joints;
   std::vector<cg3::Vec3d>     jointsRotations;
   std::vector<int>            fathers;
   std::vector<std::string>    names;

   loadSkeleton(filename.c_str(),
                joints,
                jointsRotations,
                fathers,
                names);
   c->skeleton->create(joints,
                       jointsRotations,
                       fathers,
                       names);
   c->skeleton->initPicking();
   c->glCanvas->pushDrawableObject(c->skeleton);
   c->glCanvas->pushPickableObject(c->skeleton);
   c->isSkeletonLoaded = true;
}

void importSkeletonWeights(const std::string & filename)
{
   Controller * c = Controller::get();

   //Load Skeleton Weights
   c->skeletonWeights->create(c->character->getNumVertices(),
                              c->skeleton->getNumNodes());
   loadWeights(filename.c_str(),
               c->skeletonWeights);
   c->areSkeletonWeightsLoaded = true;
   c->lbs->create(c->skeletonWeights, c->character, c->skeleton);
   c->dqs->create(c->skeletonWeights, c->character, c->skeleton);
   c->skeletonSkinning = c->lbs;
   c->isSkeletonSkinningInitialized = true;
   c->isSkeletonDeformerActive = true;
}

void importCage(const std::string & filename)
{
   Controller * c = Controller::get();

   std::string cageFilename;
   openFileSelectionDialog(cageFilename, "Load Cage", "3D Meshes (*.obj *.ply)",(QWidget*)c->mainWindow);
   if(!cageFilename.empty())
   {
      std::string cageExtension = cageFilename.substr(cageFilename.size()-4,4);
      std::string cageFilenameNoExt = cageFilename.substr(0, cageFilename.size()-4);
      std::cout << "Loading Cage File: " << cageFilename << std::endl;


      //Load Character Cage
      std::vector<double> v_cage;
      std::vector<double> uv_cage;
      std::vector<int> f_cage;

      loadMesh((cageFilename).c_str(),v_cage,uv_cage,f_cage);
      c->cage->create(v_cage,f_cage);

      c->glCanvas->pushDrawableObject(c->cage);
      c->glCanvas->pushPickableObject(c->cage);
      c->isCageLoaded = true;


      //Load Cage Weights
      std::ifstream file((cageFilenameNoExt+"_cageWeights.txt").c_str());

      if (file.is_open())
      {
         std::cout << "Load preexst. Cage Weights" << std::endl;
         c->cageWeights->create(c->character->getNumVertices(),
                                c->cage->getNumVertices());
         loadWeights((cageFilenameNoExt+"_cageWeights.txt").c_str(),
                     c->cageWeights);

         if(c->isCharacterLoaded && c->isCageLoaded)
         {

            c->nc->create(c->character, c->cage);

            c->mvc->create(c->cageWeights,
                           c->character,
                           c->cage);

            c->cageSkinning = c->mvc;

            c->areCageWeightsLoaded = true;
            c->isCageSkinningInitialized = true;
            c->isCageDeformerActive = true;
         }
      }
      else
      {
         std::cout << "File " << cageFilenameNoExt+"_cageWeights.txt" << " not found. Computing Cage Weights" << std::endl;
         file.close();
         computeCageWeights();
      }
   }
}

void importCageWeights(const std::string & filename)
{

}

void importCoR(const std::string & filename)
{
   Controller * c = Controller::get();

   //Load CoR Weights
   std::ifstream fileCoR(filename.c_str());

   if (fileCoR.is_open())
   {
      std::cout << "Load preexst. CoR Weights" << std::endl;

      c->corWeights->create(c->character->getNumVertices(),
                            c->cage->getNumVertices());

      SparseWeights corWeightsWRTMesh( c->character->getNumVertices() , c->character->getNumVertices() );

      loadSparseWeights(filename.c_str(),
                        corWeightsWRTMesh);

      *(c->corWeights) = corWeightsWRTMesh * *(c->cageWeights);


      std::cout << "!! Update CoR " << std::endl;
      if(c->isCharacterLoaded && c->isSkeletonLoaded  &&  c->isCageLoaded)
      {
         c->cor->create(c->skeletonWeights,
                        c->character,
                        c->cage,
                        c->skeleton,
                        c->corWeights);
      }

   }
   else
   {
      std::cout << "CoR Weights not found" << std::endl;
      fileCoR.close();
   }
}

void importMAXVOL(const std::string & filename)
{
   Controller * c = Controller::get();
   //Load MAXVOL selection
   std::ifstream fileMAXVOL(filename.c_str());

   if (fileMAXVOL.is_open())
   {
      loadMAXVOL(filename.c_str(),
                 c->selectedVerticesForInversion);
   }
}
