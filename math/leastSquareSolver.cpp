#include "leastSquareSolver.h"
#include <iostream>

LeastSquareSolver::LeastSquareSolver()
{
}

LeastSquareSolver::LeastSquareSolver(const Eigen::SparseMatrix<double> & A)
{
   init(A);
}

void LeastSquareSolver::init(const Eigen::SparseMatrix<double> & A)
{
   At  = A.transpose();
   AtA = At * A;
   assert(AtA.rows() == AtA.cols());
   assert(AtA.nonZeros() > 0);
   solver.compute(AtA);
   //std::cout << solver.info() << std::endl;
   assert(solver.info() == Eigen::Success);
}

void LeastSquareSolver::solve(const Eigen::VectorXd & b,
                                    Eigen::VectorXd & x
                             )
{
   Eigen::VectorXd Atb = At * b;
   x = solver.solve(Atb).eval();
}
