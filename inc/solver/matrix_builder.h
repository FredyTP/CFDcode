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
#include <math/gradient.h>
#include <mesh/mesh.h>
#include <boundary/boundary_condition.h>
namespace solver
{

class MatrixBuilder
{
public:
    MatrixBuilder();

    void buildSystem(const mesh::Mesh* pMesh,const math::GradientScheme* gradientScheme,const bc::BoundaryCondition* _bCondition, field::Field* _field);
private:
    std::vector<Eigen::Triplet<double>> _diffusive;
    std::vector<Eigen::Triplet<double>> _convective;
    std::vector<Eigen::Triplet<double>> _diffusiveBC;
    std::vector<Eigen::Triplet<double>> _convectiveBC;
    Eigen::VectorXd independent;
    Eigen::SparseMatrix<double> system;
};

}
