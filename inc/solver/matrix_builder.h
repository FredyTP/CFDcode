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
namespace solver
{

class MatrixBuilder
{
public:
    MatrixBuilder();

    

    void buildSystem(const mesh::Mesh* pMesh, 
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective, 
        const field::Fields* pField);


    Eigen::SparseMatrix<double>& getMatrix() { return systemMatrix; }
    Eigen::SparseMatrix<double>& getVolMatrix() { return _volumeMatrix; }
    Eigen::VectorXd& getVector() { return _independent; }


private:
    void buildSubMatrix(math::SystemSubmatrix* submatrix,
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective,
        const field::Fields* pField);

    const mesh::Mesh* _mesh;
    Eigen::VectorXd _independent;
    Eigen::SparseMatrix<double> systemMatrix;
    Eigen::SparseMatrix<double> _volumeMatrix;

};

}
