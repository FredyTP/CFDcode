#pragma once
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

class MatrixBuilder
{
public:
    MatrixBuilder(); 

    void buildSystem(sys::Problem* problem,field::scalarType equation);


    Eigen::SparseMatrix<double>& getMatrix() { return systemMatrix; }
    Eigen::SparseMatrix<double>& getVolMatrix() { return _volumeMatrix; }
    Eigen::VectorXd& getVector() { return _independent; }


private:
    void buildSubMatrix(math::SystemSubmatrix* submatrix, sys::Problem* problem, field::scalarType equation);

    Eigen::VectorXd _independent;
    Eigen::SparseMatrix<double> systemMatrix;
    Eigen::SparseMatrix<double> _volumeMatrix;

};

}
