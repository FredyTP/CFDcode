/*****************************************************************//**
 * \file   matrix_builder.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <solver/matrix/matrix_builder.h>
#include <field/fields.h>
#include <fstream>
#include <equation/source/source_term.h>
namespace solver
{
    MatrixBuilder::MatrixBuilder()
    {
    }

    void MatrixBuilder::buildSystem(const sys::Problem* problem)
    {
        size_t numb_equation = problem->mesh()->cells()->size();
        math::SystemSubmatrix matrix(numb_equation);

        buildSubMatrix(&matrix, problem);
        auto matrix_triplets = matrix.cellCoeficients();

        _independent = *matrix.coeficients();
        systemMatrix.data().clear();
        systemMatrix.resize(numb_equation, numb_equation);
        systemMatrix.setFromTriplets(matrix_triplets->begin(), matrix_triplets->end(),
            [](double phi1, double phi2) {
                return phi1 + phi2;
        });       

        auto volume_triplets = matrix.cellDotCoeficients();
        _volumeMatrix.data().clear();
        _volumeMatrix.resize(numb_equation, numb_equation);
        _volumeMatrix.setFromTriplets(volume_triplets->begin(), volume_triplets->end(),
            [](double phi1, double phi2) {
                return phi1 + phi2;
        });
        //std::cout << systemMatrix << std::endl;
        //std::cout << _independent << std::endl;
    }

    void MatrixBuilder::buildSubMatrix( math::SystemSubmatrix* submatrix, const sys::Problem* problem)
    {
        auto mesh = problem->mesh();
        for(auto internal_face : mesh->internalFaces())
        {
            problem->for_each_faceTerm([submatrix, internal_face, problem] (const term::FaceEquationTerm * term)
                {
                    term->calculateBothCell(submatrix, internal_face, problem->fields());
                });
        }
        for (auto boundary_face : mesh->boundaryFaces())
        {
            problem->for_each_faceTerm([submatrix, boundary_face, problem](const term::FaceEquationTerm* term)
                {
                    term->calculateOneCell(submatrix, boundary_face, problem->fields());
                });
        }
        for (auto& cell : *mesh->cells())
        {
            problem->termporalTerm()->calculateCell(submatrix, cell.get(), problem->fields());
        }
        problem->for_each_boundary([submatrix,problem](const bc::BoundaryCondition* boundaryCondition) 
            {
                boundaryCondition->calculateBoundaryCondition(submatrix, problem->fields(),0);
            });
    }
    

    /*void MatrixBuilder::buildSystem(const mesh::Mesh* pMesh,
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions, 
        term::DiffusiveTerm* pDiffusive, term::ConvectiveTerm* pConvective,
        const field::Fields* pField)
    {
        size_t numb_equation = pMesh->cells()->size();
        math::SystemSubmatrix matrix(numb_equation);

        _mesh = pMesh;
        buildSubMatrix(&matrix,_bConditions, pDiffusive, pConvective, pField);

        ///TEMPORAL///
        term::SourceTerm sourceTerm(1e8);
        mesh::MeshSelection<mesh::Cell> selection;
        selection.selectFromMesh(pMesh, [](mesh::Cell* cell)
            {
                vector2d pos = cell->position();
                vector2d center(0.01 / 4, 0.1 / 2);
                double x = pos.x();
                double y = pos.y();
                double px = center.x();
                double py = center.y();
                double a = 0.005 / 10.0;
                double b = 0.025 /10.0;
                if (std::pow((x - px) / a, 2) + std::pow((y - py) / b, 2) <= 1)
                {
                    return true;
                }
                else
                {
                    return false;
                }});
        sourceTerm.setCells(selection);
        sourceTerm.calculate(&matrix, pField);

        //TEMPORAL___DELETEEE


  
        

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
        
    }*/

    
    
}
