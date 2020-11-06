#ifndef SPARSEWEIGHTS_H
#define SPARSEWEIGHTS_H

#include <vector>
#include <unordered_map>

#include "skinning/weights.h"

class SparseWeights
{
public:
   SparseWeights();
   SparseWeights(int vertexNumber, int handleNumber);

   void create(int vertexNumber, int handleNumber);
   void clear();

   void setWeight(const int handleId, const int vertexId, const double weight);
   double getWeight(const int handleId, const int vertexId) const;
   const std::unordered_map<int,double> & getWeights(const int vertexId) const;

   Weights operator * (const Weights & other) const;

private:
   std::vector<std::unordered_map<int , double>> weights;
   int vertexNumber;
   int handleNumber;
};

#endif // SPARSEWEIGHTS_H
