#include "importFiles.h"

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>


void loadMesh    (const char            * filename,
                  std::vector<double>   & vertices,
                  std::vector<double>   & uv      ,
                  std::vector<int>      & faces   )
{
   string str(filename);
   string filetype = str.substr(str.size()-3,3);
   uv.clear();

   if ( filetype.compare("obj") == 0 ||
        filetype.compare("OBJ") == 0    )
   {
      loadOBJ(filename, vertices, faces);
   }
   else
      if ( filetype.compare("ply") == 0 ||
           filetype.compare("PLY") == 0   )
      {
         loadPLY(filename, vertices, faces);
      }
      else
         if ( filetype.compare("off") == 0 ||
              filetype.compare("OFF") == 0   )
         {
            loadOFF(filename, vertices, faces);
         }
         else
         {
            cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadMesh() : file format not supported yet " << endl;
            exit(-1);
         }
}

void loadOBJ (const char            * filename,
              std::vector<double>   & vertices,
              //std::vector<double>   & uv      ,
              std::vector<int>      & faces   )
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadOBJ() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   while (getline(file, line))
   {
      istringstream iss(line);

      string token;
      iss >> token;
      if (token.size() > 1) continue; // vn,fn  .... I don't care

      if (token[0] == 'v')
      {
         double x, y, z;
         iss >> x >> y >> z;
         vertices.push_back(x);
         vertices.push_back(y);
         vertices.push_back(z);
         //cout << "v " << x << " " << y << " " << z << endl;
      }
      else if (token[0] == 'f')
      {
         int v0, v1, v2;
         iss >> v0 >> v1 >> v2;
         faces.push_back(v0-1);
         faces.push_back(v1-1);
         faces.push_back(v2-1);
         //cout << "f " << v0 << " " << v1 << " " << v2 << endl;
      }
   }
   file.close();
}

void loadPLY ( const char     * filename,
               vector<double> & vertices,
               vector<int>    & faces)
{
   ulong vnum = 0, fnum = 0;

   ifstream file(filename);
   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load_PLY() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   ulong found;
   bool flag = false;

   //Retrieve informations from headers
   while(getline(file, line))
   {
      if (line.find("element vertex") != std::string::npos)
      {
         found = line.find_last_of(" ");
         vnum = atoi(line.substr(found+1).c_str());
      }
      if (line.find("element face") != std::string::npos)
      {
         found = line.find_last_of(" ");
         fnum = atoi(line.substr(found+1).c_str());
      }
      if (line.find("int flags") != std::string::npos)
      {
         flag = true;
      }
      if (line == "end_header")
         break;
   }

   double x,y,z;
   //double u,v,quality;
   //int flags;


   vertices.reserve(vnum*3);
   faces.reserve(fnum*3);

   //M.V = MatrixXd (vnum, 3);
   //M.F = MatrixXi (fnum, 3);
   //C.Vmesh = MatrixXd (vnum, 2);
   //C.QVmesh = VectorXi (vnum);

   //Get vertex coordinates for each vertex
   for(ulong i=0; i<vnum; i++)
   {
      getline(file, line);
      stringstream linestream(line);
      linestream >> x >> y >> z;
      //if(flag)
      //linestream >> x >> y >> z >> flags >> quality >>
      //              u >> v;
      //else
      //linestream >> x >> y >> z >> quality >>
      //              u >> v;

      //Save coordinates for each vertex
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);

      //M.V.row(i) = Vector3d(x,y,z);
      //C.Vmesh.row(i) = Vector2d(u,v);
      //C.QVmesh[i] = quality;

   }
   int tmp, v0,v1,v2;
   for(ulong i=0; i<fnum; i++)
   {
      getline(file, line);
      stringstream linestream(line);
      linestream >> tmp >> v0 >> v1 >> v2;
      faces.push_back(v0);
      faces.push_back(v1);
      faces.push_back(v2);
      //M.F.row(i) = Vector3i(v0,v1,v2);
   }
}

void loadAnimation(const char * filename,
                   std::vector<double> & t,
                   std::vector<std::vector<double> > & cageKeyframes,
                   std::vector<std::vector<cg3::Transform>> & restSkelKeyframes,
                   std::vector<std::vector<cg3::Transform>> & deformedSkelKeyframes)
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadAnimation() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   bool areKeyframeInitialized = false;
   while (getline(file, line))
   {
      istringstream iss(line);

      string token;
      iss >> token;

      std::cout << line << std::endl ;
      std::cout << token << std::endl << std::endl;

      if (token.size() > 1) continue; // vn,fn  .... I don't care

      if (token[0] == 'k')
      {
         double time;
         iss >> time;
         double slowmotionFactor = 1.0;
         t.push_back(time*slowmotionFactor);
      }
      else if (token[0] == 'c')
      {
         if(!areKeyframeInitialized)
         {
            cageKeyframes.resize(t.size());
            restSkelKeyframes.resize(t.size());
            deformedSkelKeyframes.resize(t.size());
            areKeyframeInitialized = true;
         }

         ulong i;
         double x, y, z;
         iss >> i >> x >> y >> z;
         cageKeyframes[i].push_back(x);
         cageKeyframes[i].push_back(y);
         cageKeyframes[i].push_back(z);
      }
      else if (token[0] == 'r')
      {
         if(!areKeyframeInitialized)
         {
            cageKeyframes.resize(t.size());
            restSkelKeyframes.resize(t.size());
            deformedSkelKeyframes.resize(t.size());
            areKeyframeInitialized = true;
         }

         ulong i;
         double v[16];
         iss >> i >>
               v[ 0] >>
               v[ 1] >>
               v[ 2] >>
               v[ 3] >>
               v[ 4] >>
               v[ 5] >>
               v[ 6] >>
               v[ 7] >>
               v[ 8] >>
               v[ 9] >>
               v[10] >>
               v[11] >>
               v[12] >>
               v[13] >>
               v[14] >>
               v[15];
         cg3::Transform t(v);

         restSkelKeyframes[i].push_back(t);
      }
      else if (token[0] == 'd')
      {
         if(!areKeyframeInitialized)
         {
            cageKeyframes.resize(t.size());
            restSkelKeyframes.resize(t.size());
            deformedSkelKeyframes.resize(t.size());
            areKeyframeInitialized = true;
         }

         ulong i;
         double v[16];
         iss >> i >>
               v[ 0] >>
               v[ 1] >>
               v[ 2] >>
               v[ 3] >>
               v[ 4] >>
               v[ 5] >>
               v[ 6] >>
               v[ 7] >>
               v[ 8] >>
               v[ 9] >>
               v[10] >>
               v[11] >>
               v[12] >>
               v[13] >>
               v[14] >>
               v[15];
         cg3::Transform t(v);

         deformedSkelKeyframes[i].push_back(t);
      }
   }
   file.close();
}

void loadMAXVOL (const char                * filename,
                 std::vector<int>        & selectedvertices ) {
   FILE *fp = fopen(filename, "r");

   if(!fp)
   {
      std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : read_OFF() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   ulong nv;
   fscanf(fp, "MAXVOLSELECTION %ld", &nv);

   selectedvertices.resize( nv );
   std::cout << "MAXVOLSELECTION : ";
   for( ulong vIt = 0 ; vIt < nv ; ++vIt ) {
      fscanf(fp, "%d", &(selectedvertices[vIt]));
      std::cout << selectedvertices[vIt] << "  ";
   }
   std::cout << std::endl << std::endl;

   fclose(fp);
}

void loadOFF(const char            * filename,
             std::vector<double>   & vertices,
             std::vector<int>      & faces   )
{
   setlocale(LC_NUMERIC, "en_US.UTF-8"); // makes sure "." is the decimal separator

   FILE *fp = fopen(filename, "r");

   if(!fp)
   {
      std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : read_OFF() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   int nv, npoly, dummy;

   fscanf(fp, "OFF\n");
   fscanf(fp, "%d %d %d\n", &nv, &npoly, &dummy);

   for(int i=0; i<nv; ++i)
   {
      // http://stackoverflow.com/questions/16839658/printf-width-specifier-to-maintain-precision-of-floating-point-value
      //
      double x, y, z;
      fscanf(fp, "%lf %lf %lf\n", &x, &y, &z);
      vertices.push_back(x);
      vertices.push_back(y);
      vertices.push_back(z);
   }

   for(int i=0; i<npoly; ++i)
   {
      int n_corners, v0, v1, v2;
      fscanf(fp, "%d", &n_corners);

      if (n_corners == 3)
      {
         fscanf(fp, "%d %d %d\n", &v0, &v1, &v2);
         faces.push_back(v0);
         faces.push_back(v1);
         faces.push_back(v2);
      }
      else
      {
         std::cout << "read_OFF: polygons with " << n_corners << " corners are not supported!" << std::endl;
         return;
      }
   }

   fclose(fp);
}

void loadSkeleton(const char                  * filename,
                  std::vector<cg3::Vec3d>     & jointsTranslations,
                  std::vector<cg3::Vec3d>     & jointsRotations,
                  std::vector<int>            & fathers,
                  std::vector<std::string>    & names)
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadSkeleton() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   while (getline(file, line))
   {
      istringstream iss(line);

      string token;
      iss >> token;
      if (token.size() > 1) continue; // vn,fn  .... I don't care

      if (token[0] == 'j')
      {
         int i, father;
         std::string name;
         double x, y, z, rx, ry, rz;
         iss >> i >> name >> father >> x >> y >> z /*>> rx >> ry >> rz*/;
         jointsTranslations.push_back(cg3::Vec3d(x,y,z));
         //jointsRotations.push_back(cg3::Vec3d(rx,ry,rz));
         jointsRotations.push_back(cg3::Vec3d(0,0,0));
         fathers.push_back(father);
         names.push_back(name);
      }
   }
   file.close();
}

void loadWeights(const char *filename, Weights * weights)
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadWeights() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   while (getline(file, line))
   {
      istringstream iss(line);

      ulong j, i;
      double w;
      iss >> j >> i >> w;
      weights->setWeight(j,i,w);

   }

   file.close();
}

void loadSparseWeights(const char *filename, SparseWeights & weights)
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadWeights() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   while (getline(file, line))
   {
      istringstream iss(line);

      int j, i;
      double w;
      iss >> i >> j >> w;
      {
         weights.setWeight(j,i,w);
      }

   }

   file.close();
}

void loadSkelAnimation(const char * filename,
      std::vector<double>              & t,
      std::vector<std::vector<cg3::Transform>> & skelKeyframes,
      int & animationFileVersionNumber)
{

   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadAnimation() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   bool areKeyframeInitialized = false;
   animationFileVersionNumber=1;
   while (getline(file, line))
   {
      istringstream iss(line);

      string token;
      iss >> token;

      //std::cout << line << std::endl ;
      //std::cout << token << std::endl << std::endl;

      if (token.size() > 1) continue; // vn,fn  .... I don't care

      if ((line.compare("#V2")==0) || (line.compare("t rt")==0)) { animationFileVersionNumber = 2; } else
         if ((line.compare("#V3")==0))                           { animationFileVersionNumber = 3; }

      if (token[0] == 'k')
      {
         double time;
         iss >> time;
         double slowmotionFactor = 1.0;
         t.push_back(time*slowmotionFactor);
      }
      else if (token[0] == 's')
      {
         if(!areKeyframeInitialized)
         {
            skelKeyframes.resize(t.size());
            areKeyframeInitialized = true;
         }

         ulong i;

         if(animationFileVersionNumber==1)
         {
            double v[16];
            iss >> i >>
                  v[ 0] >>
                  v[ 1] >>
                  v[ 2] >>
                  v[ 3] >>
                  v[ 4] >>
                  v[ 5] >>
                  v[ 6] >>
                  v[ 7] >>
                  v[ 8] >>
                  v[ 9] >>
                  v[10] >>
                  v[11] >>
                  v[12] >>
                  v[13] >>
                  v[14] >>
                  v[15];
            cg3::Transform T(v);

            skelKeyframes[i].push_back(T);
         }

         if(animationFileVersionNumber==2)
         {

            double v[6];
            iss >> i >>
                  v[ 0] >>
                  v[ 1] >>
                  v[ 2] >>
                  v[ 3] >>
                  v[ 4] >>
                  v[ 5];

            cg3::Transform T(
                  v[0],
                  v[1],
                  v[2],
                  v[3],
                  v[4],
                  v[5]
                  );

            skelKeyframes[i].push_back(T);
         }

         if(animationFileVersionNumber==3)
         {

            double v[6];
            iss >> i >>
                  v[ 0] >>
                  v[ 1] >>
                  v[ 2] >>
                  v[ 3] >>
                  v[ 4] >>
                  v[ 5];

            cg3::Transform T(
                  v[0],
                  v[1],
                  v[2],
                  v[3],
                  v[4],
                  v[5]
                  );

            skelKeyframes[i].push_back(T);
         }

      }
   }
   file.close();

}


void loadCageAnimation(
      const char                       * filename,
      std::vector<double>              & t,
      std::vector<std::vector<double>> & cageKeyframes)
{

   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadAnimation() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   bool areKeyframeInitialized = false;
   while (getline(file, line))
   {
      istringstream iss(line);

      string token;
      iss >> token;

      if (token.size() > 1) continue; // vn,fn  .... I don't care

      if (token[0] == 'k')
      {
         double time;
         iss >> time;
         double slowmotionFactor = 1.0;
         t.push_back(time*slowmotionFactor);
      }
      else if (token[0] == 'c')
      {
         if(!areKeyframeInitialized)
         {
            cageKeyframes.resize(t.size());
            areKeyframeInitialized = true;
         }

         ulong i;
         double x, y, z;
         iss >> i >> x >> y >> z;
         cageKeyframes[i].push_back(x);
         cageKeyframes[i].push_back(y);
         cageKeyframes[i].push_back(z);
      }
   }
   file.close();

}

void loadDoubleVec(const char * filename, std::vector<double> & params)
{
   ifstream file(filename);

   if (!file.is_open())
   {
      cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : loadDoubleVec() : couldn't open input file " << filename << endl;
      exit(-1);
   }

   string line;
   while (getline(file, line))
   {
      istringstream iss(line);

      double token;
      iss >> token;
      params.push_back(token);

   }
   file.close();
}
