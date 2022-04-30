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
     * Boundary condition of constant temperature [K].
     */
    class ConstantTemperature : public BoundaryCondition
    {
    public:
        ConstantTemperature(double _temperature_) :_temperature(_temperature_) {
            _convectiveTerm = nullptr;
            _diffusiveTerm = nullptr;
        };
        
        /**
         * Calculate constant temperature boundary condition
         * 
         * \param submatrix
         * \param cell
         * \param field
         * \param actualTime current simulation Time [s]
         */
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field, double actualTime) const
        {            
            submatrix->addCellValues(cell,math::CellValue<double>(nullptr,_temperature));
            submatrix->addCellValues(cell, math::CellValue<double>(cell, 1));
        }

    private:
        double _temperature;
    };
}
