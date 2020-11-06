#include "sparseWeights.h"

SparseWeights::SparseWeights()
{
   create(0,0);
}

SparseWeights::SparseWeights(int vertexNumber, int handleNumber)
{
   create(vertexNumber, handleNumber);
}

void SparseWeights::create(int vertexNumber, int handleNumber)
{
   weights.resize(vertexNumber);
   this->vertexNumber = vertexNumber;
   this->handleNumber = handleNumber;
}

void SparseWeights::clear()
{
   weights.clear();
   vertexNumber = 0;
   handleNumber = 0;
}

void SparseWeights::setWeight(const int handleId, const int vertexId, const double weight)
{
   if(vertexId < vertexNumber && handleId < handleNumber)
      weights[vertexId][handleId] = weight;
}

double SparseWeights::getWeight(const int handleId, const int vertexId) const
{
   std::unordered_map< int , double >::const_iterator it = weights[vertexId].find(handleId);
   if( it == weights[vertexId].end() ) return 0.0;
   return it->second;
}

const std::unordered_map<int, double> & SparseWeights::getWeights(const int vertexId) const
{
   //TO DO: check if vertexId is valid
   return weights[vertexId];
}

Weights SparseWeights::operator *(const Weights & other) const
{
   Weights res( this->vertexNumber , other.getNumberOfHandles() );
   for(int i = 0 ; i < this->vertexNumber  ; ++i)
   {
      for( int c= 0 ; c <  other.getNumberOfHandles() ; ++c)
      {
         double res_ic=0;
         std::unordered_map<int, double> const & row_i = getWeights(i);
         for(auto it = row_i.begin() ; it != row_i.end() ; ++it)
         {
            int k = it->first;
            double val_ik = it->second;
            res_ic += val_ik * other.getWeight(c , k);
         }
         res.setWeight(c, i, res_ic);
      }
   }
   return res;
}
