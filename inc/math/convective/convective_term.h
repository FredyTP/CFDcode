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
    //Generalize this concept in equation terms
    class ConvectiveTerm
    {
    public:
        
        ConvectiveTerm(SurfaceIntegral* surfaceInt, FaceInterpolation* faceInterpolation) {
            surfaceIntegralScheme = surfaceInt;
            faceInterpolationScheme = faceInterpolation;
        }
        
   
        static double GetConvectionCoeficient(const mesh::Cell* cell,const mesh::Face* face,const field::Fields* field)
        {
            const field::ScalarStateVector& statevector = field->scalarField(cell);
            double density = statevector.density;
            double cv = 1040;// cell->material()->specificHeat(statevector);
            double area = face->area();
            vector2d normal = face->getNormal(cell);
            const vector2d& velocity = field->velocityField(face);
            return density * cv * area * normal.dot(velocity);
        }

        void calculateFaceConvection(SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field) const
        {
            auto cell = face->cell1();
            double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, field);
            
            std::vector<FaceVariable> facevars = surfaceIntegralScheme->integrate(coef, face);
            for (auto & facevar : facevars) //Revisar usar referencias... MEMORY MNG!!
            {
                std::vector<CellValue<double>> cellvalues;
                faceInterpolationScheme->interpolateFace(cellvalues,facevar, field);
                submatrix->addFaceValues(face, cellvalues);
            }
        }
        void calculateFaceConvectionCell(SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field, bool isBoundaryCell = false) const
        {

            //TODO: fix coef
            double coef = ConvectiveTerm::GetConvectionCoeficient(face->cell1(), face, field);

            std::vector<FaceVariable> facevars = surfaceIntegralScheme->integrate(coef, face);
            for (auto& facevar : facevars) //Revisar usar referencias... MEMORY MNG!!
            {               
                std::vector<CellValue<double>> cellvalues;
                faceInterpolationScheme->interpolateFace(cellvalues, facevar, field);
                submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);
            }
        }
        
        
        // GETTERS
        SurfaceIntegral* getSurfaceIntegralScheme() { return surfaceIntegralScheme; }
        FaceInterpolation* getFaceInterpolation() { return faceInterpolationScheme; }
    private:
        SurfaceIntegral* surfaceIntegralScheme;
        FaceInterpolation* faceInterpolationScheme;
    };


}
}
