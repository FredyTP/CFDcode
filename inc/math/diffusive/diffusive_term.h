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
