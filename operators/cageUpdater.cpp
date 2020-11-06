#include "cageUpdater.h"

#include "Eigen/Dense"


CageUpdater::CageUpdater()
{

}

CageUpdater::CageUpdater(Weights *_w,
                         Character *_character,
                         Cage *_cage)
{
   create(_w,
          _character,
          _cage);
}

CageUpdater::CageUpdater(Weights *_w,
                         Character *_character,
                         Cage *_cage ,
                         std::vector< int > const & _selectedVertices )
{
   create(_w,
          _character,
          _cage,
          _selectedVertices);
}

bool CageUpdater::create(Weights *_w, Character *_character, Cage *_cage)
{
   clear();

   setWeights(_w);
   setCharacter(_character);
   setCage(_cage);

   Eigen::SparseMatrix<double> m;
   w->exportWeightsToEigen(m);
   solver.init(m);

   return true;
}

bool CageUpdater::create(Weights *_w, Character *_character, Cage *_cage, const std::vector<int> & _selectedVertices)
{
   clear();

   selectedVertices = _selectedVertices;

   setWeights(_w);
   setCharacter(_character);
   setCage(_cage);

   Eigen::SparseMatrix<double> m;
   w->exportWeightsToEigen(m , selectedVertices);
   solver.init(m);

   return true;
}

void CageUpdater::clear()
{
   selectedVertices.clear();

   w = nullptr;
   character = nullptr;
   cage = nullptr;

   //solver = LeastSquareSolver();
}

void CageUpdater::updatePosition()
{
   Eigen::VectorXd newVx;
   Eigen::VectorXd newVy;
   Eigen::VectorXd newVz;

   if(selectedVertices.size() == 0)
   {
      // THEN DO THE COMPUTATIONS BASED ON ALL MESH VERTICES :
      character->exportVerticesToEigen(newVx, newVy, newVz);

      Eigen::VectorXd newCageX;
      Eigen::VectorXd newCageY;
      Eigen::VectorXd newCageZ;

      solver.solve( newVx, newCageX);
      solver.solve( newVy, newCageY);
      solver.solve( newVz, newCageZ);

      unsigned long cageSize = cage->getNumVertices();
      std::vector<double> newCageStd(cageSize*3);

      for(unsigned int i=0; i<cageSize; ++i)
      {
         newCageStd[i*3+0] = newCageX[i];
         newCageStd[i*3+1] = newCageY[i];
         newCageStd[i*3+2] = newCageZ[i];
      }

      cage->setActualPoseVector(newCageStd);
   }
   else  {
      // THEN DO THE COMPUTATIONS BASED ON SELECTED MESH VERTICES ONLY:
      newVx.resize( selectedVertices.size() );
      newVy.resize( selectedVertices.size() );
      newVz.resize( selectedVertices.size() );

      std::vector< double > const & mVerts = character->getVerticesVector();
      for( unsigned int vIt = 0 ; vIt < selectedVertices.size() ; ++vIt ) {
         unsigned int selectedVertex = selectedVertices[vIt];
         newVx[ vIt ] = mVerts[ 3 * selectedVertex + 0 ];
         newVy[ vIt ] = mVerts[ 3 * selectedVertex + 1 ];
         newVz[ vIt ] = mVerts[ 3 * selectedVertex + 2 ];
      }

      Eigen::VectorXd newCageX;
      Eigen::VectorXd newCageY;
      Eigen::VectorXd newCageZ;

      solver.solve( newVx, newCageX);
      solver.solve( newVy, newCageY);
      solver.solve( newVz, newCageZ);

      unsigned long cageSize = cage->getNumVertices();
      std::vector<double> newCageStd(cageSize*3);

      for(unsigned int i=0; i<cageSize; ++i)
      {
         newCageStd[i*3+0] = newCageX[i];
         newCageStd[i*3+1] = newCageY[i];
         newCageStd[i*3+2] = newCageZ[i];
      }

      cage->setActualPoseVector(newCageStd);
   }
}
