/*****************************************************************//**
 * \file   matrix_builder.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <solver/matrix_builder.h>
#include <field/field.h>
#include <fstream>

namespace solver
{
    MatrixBuilder::MatrixBuilder()
    {
    }

    void MatrixBuilder::buildSubMatrix(math::SystemSubmatrix* submatrix, 
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective, 
        const field::Fields* pField)
    {
        pConvective->calculateConvectiveMatrix(submatrix, _mesh, pField);
        pDiffusive->calculateDiffusiveMatrix(submatrix, _mesh, pField);

        for (size_t i = 0; i < _bConditions.size(); i++)
        {
            auto boundary = _bConditions[i].get();
            boundary->getBCCoefs(submatrix, pField);
        }       
    }
    

    void MatrixBuilder::buildSystem(const mesh::Mesh* pMesh, 
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions, 
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective, 
        const field::Fields* pField)
    {
        math::SystemSubmatrix matrix;
        _mesh = pMesh;
        buildSubMatrix(&matrix,_bConditions, pDiffusive, pConvective, pField);

        //TO EIGEN MATRIX
        auto matrix_triplets = matrix.toTriplets();
        size_t numb_equation = pMesh->cells()->size();
        _independent = matrix.toVector(numb_equation);

        systemMatrix.resize(numb_equation, numb_equation);
        systemMatrix.setFromTriplets(matrix_triplets.begin(), matrix_triplets.end(),
            [](double phi1, double phi2) {
                return phi1 + phi2;
        });

        
    }

    void MatrixBuilder::solve()
    {
        Eigen::SimplicialCholesky <Eigen::SparseMatrix<double>> chol(systemMatrix);
        _solution = chol.solve(_independent);

    }

    void MatrixBuilder::save(const std::string& filename)
    {
        std::ofstream file;
        file.open(filename, std::ios::trunc);
        if (file.is_open())
        {
            file << "X" << "," << "Y" << "," << "Value" << std::endl;
            for (int i = 0; i < _solution.size(); i++)
            {

                vector2d pos = _mesh->cells()->at(i)->getCentroid();
                file << pos.x() << "," << pos.y() << "," << _solution(i) << std::endl;
            }
            file.close();
        }
    }
    
}
