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
#include <math/var/system_submatrix.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <field/field.h>


namespace term
{
    class DiffusiveTerm
    {
    public:
        DiffusiveTerm(term::GradientFlux* gradientScheme) {
            _gradientFlux = gradientScheme;
        }


        void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field) const
        {
            double diffusionCoef = 10;// DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, diffusionCoef, face);
            submatrix->addFaceValues(face, cellvalues);

                
        }
           
        void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field, bool isBoundaryCell = false) const
        {
            double diffusionCoef = 10;// DiffusiveTerm::GetDiffusionCoeficient(cell, face, field);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, diffusionCoef, face);
            submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);

        }


        double static GetDiffusionCoeficient(const mesh::Cell* cell, const mesh::Face* face, const field::Fields* field)
        {
            //For generalization this could be get from a labda function or class
            return cell->material()->conductivity(field->scalarField(cell));
        }
    private:
        term::GradientFlux* _gradientFlux;
    };
    
}
