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
    class ConstantFlux : public BoundaryCondition
    {
    public:
        ConstantFlux(double _flux_) :_flux(_flux_) {
            _convectiveTerm = nullptr;
            _diffusiveTerm = nullptr;
        };
        
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const field::Fields* field) const
        {
            _diffusiveTerm->calculateOneCell(submatrix, cell->faces()[0], field, true);
            submatrix->addConstant(math::SystemConstant(cell->index(), _flux));                      
        }

    private:
        double _flux;
    };
}

