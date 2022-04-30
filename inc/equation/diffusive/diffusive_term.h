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
    /**
     * Face term for diffusive fluxes.
     */
    class DiffusiveTerm : public FaceEquationTerm
    {
    public:
        DiffusiveTerm(term::GradientFlux* gradientScheme) {
            _gradientFlux = gradientScheme;
        }


        void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields) const
        {
            double diffusionCoef = DiffusiveTerm::GetDiffusionCoeficient(face->cell1(), face, fields);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, -diffusionCoef, face);
            submatrix->addFaceValues(face, cellvalues);

                
        }
           
        void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields, bool isBoundaryCell = false) const
        {
            double diffusionCoef = DiffusiveTerm::GetDiffusionCoeficient(face->cell1(), face, fields);
            std::vector<math::CellValue<double>> cellvalues;
            _gradientFlux->integrateGradientFace(cellvalues, -diffusionCoef, face);
            submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);

        }


        double static GetDiffusionCoeficient(const mesh::Cell* cell, const mesh::Face* face, const field::Fields* fields)
        {
            //For generalization this could be get from a labda function or class
            return cell->material()->conductivity(cell,fields);
        }

        void setGradientFlux(term::GradientFlux* gradientFlux)
        {
            _gradientFlux = gradientFlux;
        }
    private:
        term::GradientFlux* _gradientFlux;
    };
    
}
