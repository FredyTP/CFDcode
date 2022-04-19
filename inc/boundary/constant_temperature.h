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
#include <field/field.h>
#include <math/var/system_submatrix.h>

namespace bc
{
    class ConstantTemperature : public BoundaryCondition
    {
    public:
        ConstantTemperature(double _temperature_) :_temperature(_temperature_) {
            _convectiveTerm = nullptr;
            _diffusiveTerm = nullptr;
        };
        
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field) const
        {            
            submatrix->addConstant(math::SystemConstant(cell->index(), _temperature));
            submatrix->addCellVar(math::CellVariable(cell->index(), cell->index(), 1));            
        }

    private:
        double _temperature;
    };
}
