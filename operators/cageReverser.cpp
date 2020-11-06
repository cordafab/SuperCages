#include "cageReverser.h"

#include "Eigen/Dense"

#include <QTime>
#include <QElapsedTimer>

CageReverser::CageReverser()
{

}

CageReverser::CageReverser(Character * _character,
                               Cage      * _cage,
                               Skeleton  * _skel,
                               Weights   * _psi,   //ψ skelUpdater weights
                               Weights   * _phi,   //φ cage weights
                               Weights   * _omega  //ω skel weights
                               )
{
   create(_character,
          _cage,
          _skel,
          _psi,
          _phi,
          _omega
          );
}



CageReverser::CageReverser(Character * _character,
                               Cage      * _cage,
                               Skeleton  * _skel,
                               Weights   * _psi,   //ψ skelUpdater weights
                               Weights   * _phi,   //φ cage weights
                               Weights   * _omega , //ω skel weights
                               std::vector< int > const & _selectedVerticesForInversion
                               )
{
   create(_character,
          _cage,
          _skel,
          _psi,
          _phi,
          _omega,
          _selectedVerticesForInversion
          );
}

bool CageReverser::create(Character *_character,
                            Cage *_cage,
                            Skeleton *_skel,
                            Weights *_psi,
                            Weights *_phi,
                            Weights *_omega)
{
   clear();

   character = _character;
   cage = _cage;
   skel = _skel;
   psi = _psi;
   phi = _phi;
   omega = _omega;

   psi->identityKroneckerProduct(PSI);
   phi->identityKroneckerProduct(PHI);
   PHI_transpose = PHI.transpose(); // ADDED BY JM
   omega->identityKroneckerProduct(OMEGA);
   Ar.resize(3*skel->getNumNodes(),3*skel->getNumNodes());
   BtopoInverse.resize(3*skel->getNumNodes(),3*skel->getNumNodes());
   R.resize(3*character->getNumVertices(),3*character->getNumVertices());
   computeBtopoInverse();
   updateAr();
   updateR();
   updateSolver();
   refreshMatrices = false;

   return true;
}

bool CageReverser::create(Character *_character, Cage *_cage, Skeleton *_skel, Weights *_psi, Weights *_phi, Weights *_omega, const std::vector<int> &_selectedVerticesForInversion)
{
   clear();
   selectedVerticesForInversion = _selectedVerticesForInversion;
   character = _character;
   cage = _cage;
   skel = _skel;
   psi = _psi;
   phi = _phi;
   omega = _omega;
   psi->identityKroneckerProduct(PSI);
   phi->identityKroneckerProduct(PHI , selectedVerticesForInversion);
   PHI_transpose = PHI.transpose(); // ADDED BY JM
   omega->identityKroneckerProduct(OMEGA , selectedVerticesForInversion);
   Ar.resize(3*skel->getNumNodes(),3*skel->getNumNodes());
   BtopoInverse.resize(3*skel->getNumNodes(),3*skel->getNumNodes());
   R.resize(3* selectedVerticesForInversion.size() ,3* selectedVerticesForInversion.size() );
   computeBtopoInverse();
   updateAr();
   updateR();
   updateSolver();
   refreshMatrices = false;

   return true;
}

void CageReverser::clear()
{
   selectedVerticesForInversion.clear();

   character = nullptr;
   cage = nullptr;
   skel = nullptr;
   psi = nullptr;   //ψ skelUpdater weights
   phi = nullptr;  //φ cage weights
   omega = nullptr; //ω skel weights

   refreshMatrices = false;
   selectedVerticesForInversion.clear();

   PSI.resize(0,0);
   PHI.resize(0,0);
   PHI_transpose.resize(0,0);
   OMEGA.resize(0,0);
   Ar.resize(0,0);
   Btopo.resize(0,0);
   BtopoInverse.resize(0,0);
   R.resize(0,0);
}

void CageReverser::updateAr()
{
   //if(refreshMatrices)
   {
      std::vector<Eigen::Triplet<double>> ArTriplets;

      for(unsigned int j=0 ; j<skel->getNumNodes(); ++j)
      {
         bool jIsARoot = (skel->getNode(j).getFather()==-1);
         if(jIsARoot)
         {

            cg3::Transform Tj1 = skel->getNode(j).getGlobalTRest();
            cg3::Transform TjActual = skel->getNode(j).getGlobalTCurrent();
            cg3::Transform Rj = TjActual.cumulateWith(Tj1.inverse());
            //  cg3::Transform Rj = skel->getNode(j).getGlobalTransformation(); //take the first 3x3 submatrix for the rotation

            for(int c1 = 0 ; c1 < 3 ; ++c1)
            {
               for(int c2 = 0 ; c2 < 3 ; ++c2)
               {
                  if( c1 == c2 )
                  {
                     ArTriplets.push_back(Eigen::Triplet<double>(3*j+c1, 3*j+c2, 1.0-Rj(c1,c2))) ;
                  }
                  else
                  {
                     ArTriplets.push_back(Eigen::Triplet<double>(3*j+c1, 3*j+c2, 0.0-Rj(c1 , c2))) ;
                  }
               }
            }
         }
         else
         {
            int f = skel->getNode(j).getFather();

            cg3::Transform Tj1 = skel->getNode(j).getGlobalTRest();
            cg3::Transform TjActual = skel->getNode(j).getGlobalTCurrent();
            cg3::Transform Rj = TjActual.cumulateWith(Tj1.inverse());

            cg3::Transform Tf1 = skel->getNode(f).getGlobalTRest();
            cg3::Transform TfActual = skel->getNode(f).getGlobalTCurrent();
            cg3::Transform Rf = TfActual.cumulateWith(Tf1.inverse());

            for(int c1=0; c1<3; ++c1)
            {
               for(int c2=0; c2<3; ++c2)
               {
                  ArTriplets.push_back(Eigen::Triplet<double>(3*j+c1, 3*j+c2, Rj(c1 , c2) - Rf(c1 , c2)));
               }
            }
         }
      }

      Ar.setFromTriplets(ArTriplets.begin(), ArTriplets.end());
      Ar.makeCompressed();
   }
}


void CageReverser::computeBtopoInverse()
{
   std::vector<Eigen::Triplet<double>> BtopoTriplets;

   for(unsigned int j=0 ; j<skel->getNumNodes(); ++j)
   {
      bool jIsARoot = (skel->getNode(j).getFather()==-1);
      if(jIsARoot)
      {
         for( int c1 = 0 ; c1 < 3 ; ++c1 )
         {
            BtopoTriplets.push_back( Eigen::Triplet<double>(3*j+c1 , 3*j+c1 , 1.0));
         }
      }
      else
      {
         int f = skel->getNode(j).getFather();

         for(int c1=0; c1<3; ++c1)
         {
            BtopoTriplets.push_back( Eigen::Triplet<double>(3*j+c1 , 3*j+c1 , -1.0));
         }

         for(int c1=0; c1<3; ++c1)
         {
            BtopoTriplets.push_back( Eigen::Triplet<double>(3*j+c1 , 3*f+c1 , 1.0));
         }
      }
   }

   // Eigen::SparseMatrix<double> Btopo;
   Btopo.resize(3*skel->getNumNodes(), 3*skel->getNumNodes());
   Btopo.setFromTriplets(BtopoTriplets.begin(), BtopoTriplets.end());
   Btopo.makeCompressed();

   //How to compute BtopoInverse? Is this right?
   BtopoInverse = (Eigen::MatrixXd(Btopo).inverse()).sparseView();
}

void CageReverser::updateR()
{
   if( selectedVerticesForInversion.size() == 0 ) { // THEN DO COMPUTATIONS BASED ON ALL VERTICES :
      //if(refreshMatrices)
      {
         std::vector<Eigen::Triplet<double>> Rtriplets;
         for(unsigned int i=0; i<character->getNumVertices(); ++i)
         {
            double tempRi[16] = { 0 };
            cg3::Transform Ri(tempRi);
            for(unsigned int j=0; j<skel->getNumNodes(); j++)
            {
               double w = omega->getWeight(j,i);
               cg3::Transform Tj1 = skel->getNode(j).getGlobalTRest();
               cg3::Transform TjActual = skel->getNode(j).getGlobalTCurrent();
               cg3::Transform Rj = TjActual.cumulateWith(Tj1.inverse());
               Ri = Ri + (Rj*w);
            }

            for(int c1=0; c1<3; ++c1)
            {
               for(int c2=0; c2<3; ++c2)
               {
                  Rtriplets.push_back(Eigen::Triplet<double>(3*i+c1, 3*i+c2, Ri(c1 , c2)));
               }
            }
         }
         R.setFromTriplets(Rtriplets.begin(),Rtriplets.end());
         R.makeCompressed();
      }
   }
   else { // THEN DO COMPUTATIONS BASED ON SELECTED VERTICES ONLY :
      //if(refreshMatrices)
      {
         std::vector<Eigen::Triplet<double>> Rtriplets;
         for(unsigned int iIt=0; iIt<selectedVerticesForInversion.size(); ++iIt)
         {
            int i = selectedVerticesForInversion[iIt];

            double tempRi[16] = { 0 };
            cg3::Transform Ri(tempRi);
            for(unsigned int j=0; j<skel->getNumNodes(); j++)
            {
               double w = omega->getWeight(j,i);
               cg3::Transform Tj1 = skel->getNode(j).getGlobalTRest();
               cg3::Transform TjActual = skel->getNode(j).getGlobalTCurrent();
               cg3::Transform Rj = TjActual.cumulateWith(Tj1.inverse());
               Ri = Ri + (Rj*w);
            }

            for(int c1=0; c1<3; ++c1)
            {
               for(int c2=0; c2<3; ++c2)
               {
                  Rtriplets.push_back(Eigen::Triplet<double>(3*iIt+c1, 3*iIt+c2, Ri(c1 , c2)));
               }
            }
         }
         R.setFromTriplets(Rtriplets.begin(),Rtriplets.end());
         R.makeCompressed();
      }
   }
}

void CageReverser::updateSolver()
{
   Eigen::SparseMatrix<double> A;
   Eigen::SparseMatrix<double> PHIPLUS; //Pseudoinverse of PHI

   // A = /*PHIPLUS * */ ((R * PHI) + (OMEGA * BtopoInverse * Ar * PSI)); //size should be right: 3n x 3mc

   std::cout << "\t Computation of A : " << std::endl;
   A = /*PHIPLUS * */ PHI_transpose * ((R * PHI) + (OMEGA * BtopoInverse * Ar * PSI)); //  CHANGED BY JM

   // std::cout << "Diff(A , PHI): " << (A - PHI).norm() << std::endl;

   solver.init(A);
}

void CageReverser::propagateToRest()
{
   if(refreshMatrices)
   {
      QElapsedTimer timer;
      qint64 nanoSec;

      timer.start();
      updateAr();
      updateR();
      //init solver only if the skeleton has changed
      updateSolver();
      refreshMatrices = false;
      nanoSec = timer.elapsed();
      std::cout << "cage Translator updated in " << nanoSec << " mSec." << std::endl;

   }

   Eigen::VectorXd dc1;
   dc1.resize(cage->getNumVertices()*3);
   const std::vector<double> & lastTranslations = cage->getLastTranslations(); // dC'
   for(unsigned int i=0; i< cage->getNumVertices()*3;++i)
   {
      dc1(i) = lastTranslations[i];
   }

   Eigen::VectorXd dc2;

   // dc2 = PHI*dc1;
   dc2 = PHI_transpose *( PHI*dc1 ); //  CHANGED BY JM

   Eigen::VectorXd x;
   x.resize(cage->getNumVertices()*3);
   solver.solve(dc2,x); // solve for (R * Phi  +  Omega * Btopo^{-1} * Ar * Psi) * dC = (Phi * dC')



   const std::vector<double> & oldRestVertices = cage->getRestPoseVerticesVector();
   std::vector<double> newRestVertices(oldRestVertices.size());
   for(unsigned int i=0; i<newRestVertices.size(); ++i)
   {
      newRestVertices[i] = oldRestVertices[i] + x(i);
   }
   cage->setRestPoseVector(newRestVertices);
}
