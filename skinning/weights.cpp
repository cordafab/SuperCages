#include "weights.h"

Weights::Weights()
{
   create(0,0);
}

Weights::Weights(unsigned long vertexNumber, unsigned long handleNumber)
{
   create(vertexNumber, handleNumber);
}

void Weights::create(unsigned long vertexNumber, unsigned long handleNumber)
{
   weights.resize(vertexNumber);
   nonZeros.resize(vertexNumber);
   this->vertexNumber = vertexNumber;
   this->handleNumber = handleNumber;
   for(unsigned long i=0; i<vertexNumber; ++i)
      weights[i].resize(handleNumber,0.0);
}

void Weights::clear()
{
   weights.clear();
   nonZeros.clear();
   vertexNumber = 0;
   handleNumber = 0;
}

void Weights::setWeight(const unsigned long handleId, const unsigned long vertexId, const double weight)
{
   //TO DO: ASSERT
   //if(vertexId < vertexNumber && handleId < handleNumber)
   weights[vertexId][handleId] = weight;
   nonZeros[vertexId].push_back(handleId);
}

double Weights::getWeight(const unsigned long handleId, const unsigned long vertexId) const
{
   //TO DO: ASSERT
   //if(vertexId < vertexNumber && handleId < handleNumber)
   double weight = weights[vertexId][handleId];
   return weight;
}

const std::vector<double> & Weights::getWeights(const unsigned long vertexId) const
{
   //TO DO: ASSERT
   return weights[vertexId];
}

const std::vector<unsigned long> & Weights::getNonZeros(const unsigned long vertexId) const
{
   //TO DO: ASSERT
   return nonZeros[vertexId];
}

Weights Weights::operator *(const Weights & other) const
{
   Weights res( vertexNumber , handleNumber );
   for( unsigned int i = 0; i < vertexNumber ; ++i )
   {
      for( unsigned int c = 0; c < other.handleNumber ; ++c )
      {
         double res_ic=0;
         for( unsigned int k = 0; k < handleNumber ; ++k )
         {
            double val_ik = getWeight(k,i);
            res_ic += val_ik * other.getWeight(c, k);
         }
         res.setWeight(c , i , res_ic);
      }
   }
   return res;
}

void Weights::exportWeightsToEigen(Eigen::SparseMatrix<double> & m) const
{
   m.resize(vertexNumber,handleNumber);
   std::vector<Eigen::Triplet<double>> triplets;

   for( unsigned int i = 0; i < vertexNumber ; ++i )
   {
      std::vector<unsigned long> handles = getNonZeros(i);
      for(int j : handles)
      {
         double w = getWeight(j,i);
         triplets.push_back(Eigen::Triplet<double>(i, j, w));
      }
   }
   m.setFromTriplets(triplets.begin(),triplets.end());
   m.makeCompressed();
}

void Weights::exportWeightsToEigen(Eigen::SparseMatrix<double> & m , std::vector< int > const & rowsIndices) const
{
   m.resize(rowsIndices.size(),handleNumber);
   std::vector<Eigen::Triplet<double>> triplets;
   for( unsigned int i = 0; i < rowsIndices.size() ; ++i )
   {
      int iIdx = rowsIndices[i];
      std::vector<unsigned long> handles = getNonZeros(iIdx);
      for(int j : handles)
      {
         double w = getWeight(j,iIdx);
         triplets.push_back(Eigen::Triplet<double>(i, j, w));
      }
   }
   m.setFromTriplets(triplets.begin(),triplets.end());
   m.makeCompressed();
}

void Weights::identityKroneckerProduct(Eigen::SparseMatrix<double> & m) const
{
   m.resize(3*vertexNumber,3*handleNumber);
   std::vector<Eigen::Triplet<double>> triplets;

   for( unsigned int i = 0; i < vertexNumber ; ++i )
   {
      std::vector<unsigned long> handles = getNonZeros(i);
      for(int j : handles)
      {
         double w = getWeight(j,i);
         for( unsigned int k = 0 ; k < 3 ; ++k )
         {
            triplets.push_back(Eigen::Triplet<double>(3*i+k,3*j+k, w));
         }
      }
   }
   m.setFromTriplets(triplets.begin(),triplets.end());
   m.makeCompressed();
}



void Weights::identityKroneckerProduct(Eigen::SparseMatrix<double> & m  ,  std::vector< int > const & rowsIndices ) const
{
   m.resize(3*rowsIndices.size(),3*handleNumber);
   std::vector<Eigen::Triplet<double>> triplets;
   for( unsigned int i = 0; i < rowsIndices.size() ; ++i )
   {
      unsigned long iIdx = rowsIndices[i];
      std::vector<unsigned long> handles = getNonZeros(iIdx);
      for(unsigned long j : handles)
      {
         double w = getWeight(j,iIdx);
         for(unsigned long k = 0 ; k < 3 ; ++k )
         {
            triplets.push_back(Eigen::Triplet<double>(3*i+k,3*j+k, w));
         }
      }
   }
   m.setFromTriplets(triplets.begin(),triplets.end());
   m.makeCompressed();
}
