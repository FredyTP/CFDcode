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
    class ConstantTemperature : public BoundaryCondition
    {
    public:
        ConstantTemperature(double _temperature_) :_temperature(_temperature_) {
        };
        
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field, field::scalarType equation) const
        {            
            if (equation == field::velocity_y)
            {
               submatrix->addCellValues(cell,math::CellValue<double>(nullptr,_temperature));

            }
               submatrix->addCellValues(cell, math::CellValue<double>(cell, 1));
        }

    private:
        double _temperature;
    };
    class VelocityInlet : public BoundaryCondition
    {
    public:
        VelocityInlet(double _vy_) :_vy(_vy_) {
        };

        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* field,field::scalarType equation) const
        {
            submatrix->addCellValues(cell, math::CellValue<double>(nullptr, _vy));
            submatrix->addCellValues(cell, math::CellValue<double>(cell, 1));
        }

    private:
        double _vy;
    };
}
