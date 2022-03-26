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
            void calculateDiffusiveMatrix(SystemSubmatrix* submatrix,const mesh::Mesh* _mesh, const field::Fields* field)
            {
                
                for (size_t cellid = 0; cellid < _mesh->cells()->size(); cellid++)
                {
                    calculateDiffusiveEq(submatrix,_mesh->cells()->at(cellid).get(), field);
                }
              
            }
            void calculateDiffusiveEq(SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field)
            {
                for (auto face : cell->faces())
                {
                    if (!face->isBoundary())
                    {
                        calculateFaceDiffusion(submatrix,cell, face, field);
                        
                    }

                }
            }
            void calculateFaceDiffusion(SystemSubmatrix* submatrix,const mesh::Cell* cell, const mesh::Face* face, const field::Fields* field) const
            {
                double diffusionCoef = DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
                _gradientFlux->integrateGradient(submatrix,diffusionCoef, cell, face);
                
            }
            void calculateFaceDiffusionBoundary(SystemSubmatrix* submatrix,const mesh::Cell* cell, const mesh::Face* face,double boundaryValue,const field::Fields* field) const
            {
                double diffusionCoef = DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
                _gradientFlux->integrateGradientBoundary(submatrix,diffusionCoef, cell, boundaryValue, face);
                
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
