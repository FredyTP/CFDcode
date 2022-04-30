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

#include <equation/convective/face_interpolation.h>
#include <equation/equation_term.h>
#include <math/var/system_submatrix.h>
#include <field/fields.h>

namespace term
{

    /**
     * Face term for convective fluxes.
     */
    class ConvectiveTerm : public FaceEquationTerm
    {
    public:
        
        ConvectiveTerm(FaceInterpolation* faceInterpolation) {
            faceInterpolationScheme = faceInterpolation;
        }
        
   
        static double GetConvectionCoeficient(const mesh::Cell* cell,const mesh::Face* face,const field::Fields* fields)
        {
            double density = fields->scalarField(field::density, cell);;
            double cv = cell->material()->specificHeat(cell,fields);
            double area = face->area();
            vector2d normal = face->getNormal(cell);
            const vector2d& velocity = fields->velocityField(face);
            return density * cv * area * normal.dot(velocity);
        }

        void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields) const
        {
            auto cell = face->cell1();
            double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, fields);
            math::FaceValue<double> facevar(face, coef);
            std::vector<math::CellValue<double>> cellvalues;
            faceInterpolationScheme->interpolateFace(cellvalues,facevar, fields);
            submatrix->addFaceValues(face, cellvalues);
 
        }
        void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields, bool isBoundaryCell = false) const
        {

            auto cell = face->cell1();
            double coef = ConvectiveTerm::GetConvectionCoeficient(cell, face, fields);
            math::FaceValue<double> facevar(face, coef);
            std::vector<math::CellValue<double>> cellvalues;
            faceInterpolationScheme->interpolateFace(cellvalues, facevar, fields);
            submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);
        }
        
        FaceInterpolation* getFaceInterpolation() { return faceInterpolationScheme; }

        void setFaceInterpolation(FaceInterpolation* faceInterpolation)
        {
            faceInterpolationScheme = faceInterpolation;
        }
    private:
       
        FaceInterpolation* faceInterpolationScheme;
    };



}
