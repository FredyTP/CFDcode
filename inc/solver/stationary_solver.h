#pragma once
/*****************************************************************//**
 * \file   stationary_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <solver/matrix/matrix_builder.h>
#include <Eigen/SparseLU>
#include <math/interpolation/gradient_interpolation.h>
#include <tool/chronometer.h>
namespace solver
{
    class StationarySolver
    {
    public:
        StationarySolver(BaseMatrixBuilder* _builder_) : _builder(_builder_) {}

        void solve(const sys::Problem* problem)
        {
            _builder->buildSystem(problem);

            Eigen::SparseLU<Eigen::SparseMatrix<double>> solver(_builder->getMatrix());
            _solution = solver.solve(_builder->getVector());
        }
        Eigen::VectorXd& solution() { return _solution; }
        
    private:
        Eigen::VectorXd _solution;
        BaseMatrixBuilder* _builder;
    };
}
