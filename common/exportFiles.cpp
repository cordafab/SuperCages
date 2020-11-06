#include "exportFiles.h"

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "geom/transform.h"

void saveMesh(const char                * filename,
              const std::vector<double> & vertices,
              const std::vector<int>    & faces)
{
   string str(filename);
   string filetype = str.substr(str.size()-3,3);

   if ( filetype.compare("obj") == 0 ||
        filetype.compare("OBJ") == 0    )
   {
      saveOBJ(filename, vertices, faces);
   }
   else
   if ( filetype.compare("ply") == 0 ||
        filetype.compare("PLY") == 0   )
   {
      savePLY(filename, vertices, faces);
   }
   else
   if ( filetype.compare("off") == 0 ||
        filetype.compare("OFF") == 0   )
   {
      //loadOFF(filename, vertices, faces);
   }
   else
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveMesh() : file format not supported yet " << endl;
      exit(-1);
   }
}

void saveOBJ ( const char                 * filename ,
               const std::vector<double>  & vertices ,
               const std::vector<int>     & faces    )
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveOBJ() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for(int i=0; i<(int)vertices.size(); i+=3)
   {
      fp << "v " << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << endl;
   }

   for(int i=0; i<(int)faces.size(); i+=3)
   {
      fp << "f " << faces[i]+1 << " " << faces[i+1]+1 << " " << faces[i+2]+1 << endl;
   }

   fp.close();
}


void savePLY(const char                *filename,
             const std::vector<double> & vertices,
             const std::vector<int>    & faces)
{

}


void saveWeights(const char    * filename,
                 const Weights * weights)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveWeights() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for( int i = 0; i < weights->getNumberOfVertices(); i++ )
   {

      for( int j = 0; j < weights->getNumberOfHandles(); j++ )
      {
         double w = weights->getWeight(j,i);
         if(w != 0.0)
         {
            fp << j << " " << i << " " << w << std::endl;
         }

      }
   }

   fp.close();
}

void saveAnimation(const char * filename,
                   const std::vector<double> & t,
                   const std::vector<std::vector<double>> & cageKeyframes,
                   const std::vector<std::vector<cg3::Transform>> & restSkelKeyframes,
                   const std::vector<std::vector<cg3::Transform>> & deformedSkelKeyframes)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveWeights() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for( unsigned long  i = 0; i < t.size(); ++i )
   {
      fp << "k " << t[i] << endl;   //keyframe
   }

   for( unsigned long i = 0; i < t.size(); ++i )
   {
      for( unsigned long j = 0; j < cageKeyframes[i].size(); j+=3 )
      {

         fp << "c "  //cage
            << i << " "
            << cageKeyframes[i][j+0] << " "
            << cageKeyframes[i][j+1] << " "
            << cageKeyframes[i][j+2] << endl;
      }

      for( unsigned long j = 0; j < restSkelKeyframes[i].size(); ++j )
      {

         std::vector<double> kf(16);
         restSkelKeyframes[i][j].data(kf);

         fp << "r "  //skel
            << i << " "
            << kf[0] << " "  << kf[1] << " "  << kf[2]  << " " << kf[3]  << " "
            << kf[4] << " "  << kf[5] << " "  << kf[6]  << " " << kf[7]  << " "
            << kf[8] << " "  << kf[9] << " "  << kf[10] << " " << kf[11] << " "
            << kf[12] << " " << kf[13] << " " << kf[14] << " " << kf[15] <<
            std::endl;

         //std::cout << kfT.getEigenTransformation() << std::endl;
      }

      for( unsigned long  j = 0; j < deformedSkelKeyframes[i].size(); ++j )
      {

         std::vector<double> kf(16);
         deformedSkelKeyframes[i][j].data(kf);

         fp << "d "  //skel
            << i << " "
            << kf[0] << " "  << kf[1] << " "  << kf[2]  << " " << kf[3]  << " "
            << kf[4] << " "  << kf[5] << " "  << kf[6]  << " " << kf[7]  << " "
            << kf[8] << " "  << kf[9] << " "  << kf[10] << " " << kf[11] << " "
            << kf[12] << " " << kf[13] << " " << kf[14] << " " << kf[15] <<
            std::endl;

         //std::cout << kfT.getEigenTransformation() << std::endl;
      }
   }
   fp.close();
}

void saveSkeleton (const char                        * filename,
                   const std::vector<cg3::Vec3d>     & joints,
                   const std::vector<int>            & fathers,
                   const std::vector<std::string>    & names)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveSkeleton() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for( unsigned long i = 0; i < joints.size(); ++i )
   {
      fp << "j "
         << i << " "
         << names[i] << " "
         << fathers[i] << " "
         << joints[i].x() << " "
         << joints[i].y() << " "
         << joints[i].z()
         << std::endl;
   }

   fp.close();
}

void saveSkelAnimation (
                    const char                              * filename,
                    const std::vector<double>               & t,
                    const std::vector<std::vector<cg3::Transform>> & skelKeyframes)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed


   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveWeights() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   fp << "t rt" << std::endl;

   for( unsigned long  i = 0; i < t.size(); ++i )
   {
      fp << "k " << t[i] << std::endl;   //keyframe
   }

   for( unsigned long i = 0; i < t.size(); ++i )
   {

      for( unsigned long j = 0; j < skelKeyframes[i].size(); ++j )
      {
         cg3::Vec3d r = skelKeyframes[i][j].getRotation().toEuler();
         cg3::Vec3d t = skelKeyframes[i][j].getTranslation();

         fp << "s "  //skel
            << i << " "
            << r[0] << " "  << r[1] << " "  << r[2]  << " "
            << t[0] << " "  << t[1] << " "  << t[2]  << " " <<
            std::endl;
      }
   }
   fp.close();
}

void saveCageAnimation (
                    const char                              * filename,
                    const std::vector<double>               & t,
                    const std::vector<std::vector<double> > & cageKeyframes)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveWeights() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for( unsigned long  i = 0; i < t.size(); ++i )
   {
      fp << "k " << t[i] << endl;   //keyframe
   }

   for( unsigned long i = 0; i < t.size(); ++i )
   {
      for( unsigned long j = 0; j < cageKeyframes[i].size(); j+=3 )
      {

         fp << "c "  //cage
            << i << " "
            << cageKeyframes[i][j+0] << " "
            << cageKeyframes[i][j+1] << " "
            << cageKeyframes[i][j+2] << endl;
      }
   }
   fp.close();
}

void saveDoubleVec(const char * filename, const std::vector<double> & params)
{
   ofstream fp;
   fp.open (filename);
   fp.precision(6);
   fp.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed

   if(!fp)
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : saveDoubleVec() : couldn't open output file " << filename << endl;
      exit(-1);
   }

   for( unsigned long  i = 0; i < params.size(); ++i )
   {
      fp << params[i] << endl;   //keyframe
   }

   fp.close();
}
