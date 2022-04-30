/*****************************************************************//**
 * \file   constant_temperature.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <boundary/boundary_condition.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <field/fields.h>
#include <math/var/system_submatrix.h>

namespace bc
{
    /**
     * \brief Boundary condition of constant heat flux [W/m^2].
     */
    class ConstantFlux : public BoundaryCondition
    {
    public:
        ConstantFlux(double _flux_) : _flux(_flux_) {
            _convectiveTerm = nullptr;
            _diffusiveTerm = nullptr;
        };
        
        /**
         * Overriden function to apply constant heat flux boundary condition.
         * 
         * \param submatrix
         * \param cell
         * \param field
         * \param actualTime current simulation Time [s]
         */
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field, double actualTime) const
        {
            _diffusiveTerm->calculateOneCell(submatrix, cell->faces()[0], field, true);
            submatrix->addCellValues(cell, math::CellValue<double>(nullptr, _flux));
        }

    private:
        double _flux;
    };
}

