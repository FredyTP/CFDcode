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
#include <math/volume/volume_integral.h>

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
        for(auto internal_face : _mesh->internalFaces())
        {
            pDiffusive->calculateFaceDiffusion(submatrix, internal_face, pField);
            pConvective->calculateFaceConvection(submatrix, internal_face, pField);
        }
        for (auto boundary_face : _mesh->boundaryFaces())
        {
            pDiffusive->calculateFaceDiffusionCell(submatrix, boundary_face, pField);
            pConvective->calculateFaceConvectionCell(submatrix, boundary_face, pField);
        }

        for (size_t i = 0; i < _bConditions.size(); i++)
        {
            auto boundary = _bConditions[i].get();
            boundary->calculateBoundaryCondition(submatrix, pField);
        }       
    }
    

    void MatrixBuilder::buildSystem(const mesh::Mesh* pMesh, 
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions, 
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective, 
        const field::Fields* pField)
    {
        size_t numb_equation = pMesh->cells()->size();
        math::SystemSubmatrix matrix(numb_equation);

        _mesh = pMesh;
        buildSubMatrix(&matrix,_bConditions, pDiffusive, pConvective, pField);

        

        //TO EIGEN MATRIX
        auto matrix_triplets = matrix.toTriplets();
        
        _independent = matrix.toVector();
        systemMatrix.data().clear();
        systemMatrix.resize(numb_equation, numb_equation);
        systemMatrix.setFromTriplets(matrix_triplets.begin(), matrix_triplets.end(),
            [](double phi1, double phi2) {
                return phi1 + phi2;
        });
        math::SystemSubmatrix volSystem(numb_equation);
        math::volume::VolumeIntegral volIntegral;
        volIntegral.integrateMesh(&volSystem, pMesh, pField);

        auto volume_triplets = volSystem.toTriplets();
        _volumeMatrix.data().clear();
        _volumeMatrix.resize(numb_equation, numb_equation);
        _volumeMatrix.setFromTriplets(volume_triplets.begin(), volume_triplets.end(),
            [](double phi1, double phi2) {
                return phi1 + phi2;
        });

        //std::cout << systemMatrix << std::endl;
        //std::cout << _independent << std::endl;
        
    }

    
    
}
