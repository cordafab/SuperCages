#ifndef CAGETRANSLATOR_H
#define CAGETRANSLATOR_H

#include "rigs/character.h"

#include "rigs/cage.h"

#include "rigs/skeleton.h"

#include "skinning/weights.h"

#include "Eigen/Sparse"
#include "math/leastSquareSolver.h"

class CageReverser
{
public:

    CageReverser();
    CageReverser(Character * _character,
                   Cage      * _cage,
                   Skeleton  * _skel,
                   Weights   * _psi,   //ψ skelUpdater weights
                   Weights   * _phi,   //φ cage weights
                   Weights   * _omega  //ω skel weights
                   );
    CageReverser(Character * _character,
                   Cage      * _cage,
                   Skeleton  * _skel,
                   Weights   * _psi,   //ψ skelUpdater weights
                   Weights   * _phi,   //φ cage weights
                   Weights   * _omega , //ω skel weights
                   std::vector< int >  const &    _selectedVerticesForInversion
                   );

    bool create   (Character * _character,
                   Cage      * _cage,
                   Skeleton  * _skel,
                   Weights   * _psi,   //ψ skelUpdater weights
                   Weights   * _phi,   //φ cage weights
                   Weights   * _omega  //ω skel weights
                   );
    bool create   (Character * _character,
                   Cage      * _cage,
                   Skeleton  * _skel,
                   Weights   * _psi,   //ψ skelUpdater weights
                   Weights   * _phi,   //φ cage weights
                   Weights   * _omega , //ω skel weights
                   std::vector< int >  const &    _selectedVerticesForInversion
                   );

    void clear();

   inline void skeletonEdited() { refreshMatrices = true;}

   void updateMatrices();
   void updateAr();
   void computeBtopoInverse();
   void updateR();
   void updateSolver();
   void propagateToRest();


   Eigen::SparseMatrix<double> const & getAr() const { return Ar; }
   Eigen::SparseMatrix<double> const & getBtopo() const { return Btopo; }
   Eigen::SparseMatrix<double> const & getBtopoInverse() const { return BtopoInverse; }

private:
   Character * character;
   Cage      * cage;
   Skeleton  * skel;

   Weights   * psi;   //ψ skelUpdater weights
   Weights   * phi;   //φ cage weights
   Weights   * omega; //ω skel weights

   bool refreshMatrices;

   std::vector< int >         selectedVerticesForInversion;

   Eigen::SparseMatrix<double> PSI;
   Eigen::SparseMatrix<double> PHI;
   Eigen::SparseMatrix<double> PHI_transpose;
   Eigen::SparseMatrix<double> OMEGA;
   Eigen::SparseMatrix<double> Ar;
   Eigen::SparseMatrix<double> Btopo;
   Eigen::SparseMatrix<double> BtopoInverse;
   Eigen::SparseMatrix<double> R;

   LeastSquareSolver solver;
};

#endif // CAGETRANSLATOR_H
