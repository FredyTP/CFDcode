#pragma once
/*****************************************************************//**
 * \file   stationary_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <solver/matrix_builder.h>
#include <Eigen/SparseLU>
#include <math/interpolation/gradient_interpolation.h>

namespace solver
{
    class StationarySolver
    {
    public:
        StationarySolver(MatrixBuilder* _builder_) : _builder(_builder_) {}

        void solve(sys::Problem* problem)
        {
            _builder->buildSystem(problem);

            Eigen::SparseLU<Eigen::SparseMatrix<double>> chol(_builder->getMatrix());
            _solution = chol.solve(_builder->getVector());
            std::string filename = "static_solutionOC";          

            //initfile(filename+".txt");
            //save(filename + ".txt", _solution, problem->mesh());
            //save_contour(filename + ".txt", filename + ".jpg", 1920, 1080);
                
        }
        Eigen::VectorXd& solution() { return _solution; }
        
    private:
        Eigen::VectorXd _solution;
        MatrixBuilder* _builder;
    };
}
