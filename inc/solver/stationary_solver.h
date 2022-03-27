/*****************************************************************//**
 * \file   stationary_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <solver/matrix_builder.h>
#include <Eigen/SparseLU>

namespace solver
{
    class StationarySolver
    {
    public:
        StationarySolver(MatrixBuilder* _builder_) : _builder(_builder_) {}

        void solve(const mesh::Mesh* pMesh,
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective,
        const field::Fields* pField)
        {
            _builder->buildSystem(pMesh, _bConditions, pDiffusive, pConvective, pField);

            //Eigen::MatrixXd matrix(_builder->getMatrix());
            //_solution = matrix.inverse() * _builder->getVector();
            Eigen::SparseLU<Eigen::SparseMatrix<double>> chol(_builder->getMatrix());
            _solution = chol.solve(_builder->getVector());
                
        }
        Eigen::VectorXd solution() { return _solution; }
    private:
        Eigen::VectorXd _solution;
        MatrixBuilder* _builder;
    };
}
