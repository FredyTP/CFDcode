#pragma once
/*****************************************************************//**
 * \file   volume_integral.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <math/var/system_submatrix.h>
#include <equation/equation_term.h>
#include <mesh/base/cell.h>
#include <mesh/mesh.h>
#include <field/field.h>
namespace term
{

    class TemporalTerm : public CellEquationTerm
    {
    public:
        TemporalTerm(){}

        virtual void calculateCell(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* field) const
        {          
            double coef = TemporalTerm::GetVolumeCoef(cell, field);
            submatrix->addCellDotValues(cell,math::CellValue<double>(cell, coef * cell->volume()));
        }

        static double GetVolumeCoef(const mesh::Cell* cell, const field::Fields* fields)
        {
            return cell->material()->specificHeat(fields->scalarField(cell)) * fields->scalarField(cell).density;
        }
    };
   
}
