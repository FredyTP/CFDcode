/*****************************************************************//**
 * \file   diffusive_term.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//CFD
#include <math/diffusive/gradient_flux.h>
#include <math/var/system_submatrix.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <field/field.h>


namespace math
{
    namespace diffusive
    {
        class DiffusiveTerm
        {
        public:
            DiffusiveTerm(GradientFlux* gradientScheme) {
                _gradientFlux = gradientScheme;
            }


            void calculateFaceDiffusion(SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field) const
            {
                double diffusionCoef = 10;// DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
                std::vector<CellValue<double>> cellvalues;
                _gradientFlux->integrateGradientFace(cellvalues, diffusionCoef, face);
                submatrix->addFaceValues(face, cellvalues);

                
            }
           
            void calculateFaceDiffusionCell(SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field, bool isBoundaryCell = false) const
            {
                double diffusionCoef = 10;// DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
                std::vector<CellValue<double>> cellvalues;
                _gradientFlux->integrateGradientFace(cellvalues, diffusionCoef, face);
                submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);

            }


            double static GetDiffusionCoeficient(const mesh::Cell* cell, const mesh::Face* face, const field::Fields* field)
            {
                //For generalization this could be get from a labda function or class
                return cell->material()->conductivity(field->scalarField(cell));
            }
        private:
            GradientFlux* _gradientFlux;
        };
    }
}
/*
SystemSubmatrix* calculateConvectiveMatrix(mesh::Mesh* _mesh, field::Field* field)
{
    for (size_t cellid = 0; cellid < _mesh->cells()->size(); cellid++)
    {
        calculateConvectiveEq(_mesh->cells()->at(cellid).get(), field);
    }
    return systemSubmatrix.get();
}
static double GetConvectionCoeficient(const mesh::Cell* cell, const mesh::Face* face, const field::Field* field)
{
    const field::StateVector& statevector = field->scalarField(cell);
    double density = statevector.density;
    double cv = cell->material()->specificHeat(statevector);
    double area = face->area();
    vector2d normal = face->getNormal(cell);
    const vector2d& velocity = field->velocityField(face);
    return density * cv * area * normal.dot(velocity);
}
SystemSubmatrix calculateFaceConvection(mesh::Cell* cell, mesh::Face* face, field::Field* field) const
{
    SystemSubmatrix retMatrix;
    double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, field);

    std::vector<FaceVariable> facevars = surfaceIntegralScheme->integrate(coef, cell, face);
    for (auto facevar : facevars) //Revisar usar referencias... MEMORY MNG!!
    {
        SystemSubmatrix result = faceInterpolationScheme->interpolate(facevar, field);
        retMatrix.merge(result);
    }
    return retMatrix;
}

void calculateConvectiveEq(mesh::Cell* cell, field::Field* field)
{
    for (auto face : cell->faces())
    {
        SystemSubmatrix result = calculateFaceConvection(cell, face, field);
        systemSubmatrix->merge(result);
    }
}
SurfaceIntegral* getSurfaceIntegralScheme() { return surfaceIntegralScheme.get(); }
FaceInterpolation* getFaceInterpolation() { return faceInterpolationScheme.get(); }*/
