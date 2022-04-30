/*****************************************************************//**
 * \file   matrix_builder.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <mesh/mesh.h>
#include <boundary/boundary_condition.h>
#include <system/problem.h>

namespace solver
{
class BaseMatrixBuilder
{
public:
    BaseMatrixBuilder(){}
    virtual void buildSystem(const sys::Problem* problem)=0;

    Eigen::SparseMatrix<double>& getMatrix() { return systemMatrix; }
    Eigen::SparseMatrix<double>& getVolMatrix() { return _volumeMatrix; }
    Eigen::VectorXd& getVector() { return _independent; }
protected:
    Eigen::VectorXd _independent;
    Eigen::SparseMatrix<double> systemMatrix;
    Eigen::SparseMatrix<double> _volumeMatrix;
};

class MatrixBuilder : public BaseMatrixBuilder
{
public:
    MatrixBuilder(); 

    void buildSystem(const sys::Problem* problem);


    Eigen::SparseMatrix<double>& getMatrix() { return systemMatrix; }
    Eigen::SparseMatrix<double>& getVolMatrix() { return _volumeMatrix; }
    Eigen::VectorXd& getVector() { return _independent; }
private:
    void buildSubMatrix(math::SystemSubmatrix* submatrix,const sys::Problem* problem);

};

}
