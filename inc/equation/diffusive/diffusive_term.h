/*****************************************************************//**
 * \file   diffusive_term.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//CFD
#include <equation/diffusive/gradient_flux.h>
#include <equation/equation_term.h>
#include <math/var/system_submatrix.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <field/fields.h>


namespace term
{
    class DiffusiveTerm : public FaceEquationTerm
    {
    public:
        DiffusiveTerm(term::GradientFlux* gradientScheme, field::scalarType scalarField) : FaceEquationTerm(scalarField) {
            _gradientFlux = gradientScheme;          
            if (scalarField == field::temperature)
            {
                _GetDiffusiveCoeficient = DiffusiveTerm::GetTemperatureDiffusionCoeficient;

            }
            else if (scalarField == field::velocity_x)
            {
                _GetDiffusiveCoeficient = DiffusiveTerm::GetTemperatureDiffusionCoeficient;
            }
            else if (scalarField == field::velocity_y)
            {
                _GetDiffusiveCoeficient = DiffusiveTerm::GetTemperatureDiffusionCoeficient;
            }
            else 
            {
                _GetDiffusiveCoeficient = DiffusiveTerm::GetNullCoeficient;
            }
         
        }


        void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields) const
        {
            double diffusionCoef = _GetDiffusiveCoeficient(face, fields);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, -diffusionCoef, face);
            submatrix->addFaceValues(face, cellvalues);
                
        }
           
        void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields, bool isBoundaryCell = false) const
        {
            double diffusionCoef = _GetDiffusiveCoeficient(face, fields);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, -diffusionCoef, face);
            submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);

        }

        double static GetNullCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            return 0.0;
        }
        double static GetTemperatureDiffusionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            //For generalization this could be get from a labda function or class
            double k1= face->cell1()->material()->conductivity(face->cell1(), fields);
            double k2= face->cell2()->material()->conductivity(face->cell2(), fields);

            return (k1 + k2) / 2;
        } 
        double static GetContinuityDiffusionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            //For generalization this could be get from a labda function or class


            return 1;
        }

        void setGradientFlux(term::GradientFlux* gradientFlux)
        {
            _gradientFlux = gradientFlux;
        }
    private:
        term::GradientFlux* _gradientFlux;
        std::function<double(const mesh::Face*, const field::Fields*)> _GetDiffusiveCoeficient;
    };
    
}
