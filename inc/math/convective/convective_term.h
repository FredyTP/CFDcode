/*****************************************************************//**
 * \file   convective_term.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <memory>

 //CFD
#include <math/convective/surface_intergral.h>
#include <math/convective/face_interpolation.h>

#include <math/var/system_submatrix.h>
#include <field/field.h>

namespace math
{
namespace convective
{
    class ConvectiveTerm
    {
    public:
        
        ConvectiveTerm(SurfaceIntegral* surfaceInt, FaceInterpolation* faceInterpolation) {
            surfaceIntegralScheme = surfaceInt;
            faceInterpolationScheme = faceInterpolation;
        }
        
        void calculateConvectiveMatrix(SystemSubmatrix* submatrix,const mesh::Mesh* _mesh, const field::Fields* field)
        {        
            for (size_t cellid = 0; cellid < _mesh->cells()->size(); cellid++)
            {
                calculateConvectiveEq(submatrix,_mesh->cells()->at(cellid).get(), field);
            }
        }
        static double GetConvectionCoeficient(const mesh::Cell* cell,const mesh::Face* face,const field::Fields* field)
        {
            const field::ScalarStateVector& statevector = field->scalarField(cell);
            double density = statevector.density;
            double cv = cell->material()->specificHeat(statevector);
            double area = face->area();
            vector2d normal = face->getNormal(cell);
            const vector2d& velocity = field->velocityField(face);
            return density * cv * area * normal.dot(velocity);
        }
        void calculateFaceConvection(SystemSubmatrix* submatrix,const mesh::Cell* cell, const mesh::Face* face, const field::Fields* field) const
        {
            
            double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, field);

            std::vector<FaceVariable> facevars = surfaceIntegralScheme->integrate(coef, cell, face);
            for (auto & facevar : facevars) //Revisar usar referencias... MEMORY MNG!!
            {
                faceInterpolationScheme->interpolate(submatrix,facevar, field);
            }
        }
        void calculateFaceConvectionBoundary(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const mesh::Face* face,
            double boundaryValue, const field::Fields* field) const
        {
          
            double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, field);
            std::vector<FaceVariable> facevars = surfaceIntegralScheme->integrate(coef, cell, face);
            for (auto & facevar : facevars) //Revisar usar referencias... MEMORY MNG!!
            {
                faceInterpolationScheme->interpolateBCValue(submatrix, facevar, field, boundaryValue);
            }
        }
        void calculateConvectiveEq(SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field)
        {
            for (auto face : cell->faces())
            {
                if (!face->isBoundary())
                {
                    calculateFaceConvection(submatrix, cell, face, field);
                }
            }
        }
        SurfaceIntegral* getSurfaceIntegralScheme() { return surfaceIntegralScheme; }
        FaceInterpolation* getFaceInterpolation() { return faceInterpolationScheme; }
    private:
        SurfaceIntegral* surfaceIntegralScheme;
        FaceInterpolation* faceInterpolationScheme;
    };


}
}
