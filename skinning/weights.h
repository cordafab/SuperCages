#ifndef WEIGHTS_H
#define WEIGHTS_H

#include <vector>
#include "Eigen/SparseCore"

class Weights
{
public:
    Weights();
    Weights(unsigned long vertexNumber, unsigned long handleNumber);

    void create(unsigned long vertexNumber, unsigned long handleNumber);
    void clear();

    void setWeight(const unsigned long handleId, const unsigned long vertexId, const double weight);
    double getWeight(const unsigned long handleId, const unsigned long vertexId) const;
    const std::vector<double> & getWeights(const unsigned long vertexId) const;
    const std::vector<unsigned long> & getNonZeros(const unsigned long vertexId) const;

    //double getWeightsSum(const int vertexId) const;
    //void forcePartitionOfUnity();

    inline unsigned long getNumberOfVertices() const { return vertexNumber; }
    inline unsigned long getNumberOfHandles()  const { return handleNumber; }

    Weights operator * (const Weights & other) const;

    void exportWeightsToEigen(Eigen::SparseMatrix<double> & m) const;
    void exportWeightsToEigen(Eigen::SparseMatrix<double> & m , std::vector<int> const & rowsIndices) const ;
    void identityKroneckerProduct(Eigen::SparseMatrix<double> & m) const;
    void identityKroneckerProduct(Eigen::SparseMatrix<double> & m, const std::vector<int> &rowsIndices) const;


private:
    std::vector<std::vector<double>> weights;
    std::vector<std::vector<unsigned long>> nonZeros;
    unsigned long vertexNumber;
    unsigned long handleNumber;
};

#endif // WEIGHTS_H
