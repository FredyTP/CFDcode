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
        VelocityInlet(vector2d _velocity_) :_velocity(_velocity_) {
        };

        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* field,field::scalarType equation) const
        {
            if (equation == field::velocity_x)
            {
                submatrix->addCellValues(cell, math::CellValue<double>(nullptr, _velocity.x()));
                submatrix->addCellValues(cell, math::CellValue<double>(cell, 1));
            }
            else if (equation == field::velocity_y)
            {
                submatrix->addCellValues(cell, math::CellValue<double>(nullptr, _velocity.y()));
                submatrix->addCellValues(cell, math::CellValue<double>(cell, 1));
            }
            else if (equation == field::continuity_x)
            {

                //submatrix->addCellValues(cell, math::CellValue<double>(nullptr, flux));
                submatrix->addCellValues(cell, math::CellValue<double>(cell, cell->faces()[0]->normal2().x()));
            }
            else if (equation == field::continuity_y)
            {
                //submatrix->addCellValues(cell, math::CellValue<double>(nullptr, flux));
                submatrix->addCellValues(cell, math::CellValue<double>(cell, cell->faces()[0]->normal2().y()));
            }
        }

    private:
        vector2d _velocity;
    };
    class VelocityOutlet : public BoundaryCondition
    {
    public:
        VelocityOutlet() {
        };

        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* field, field::scalarType equation) const
        {
            field::scalarType used_equation;
            if (equation == field::continuity_x)
            {
                used_equation = field::velocity_x;
                return;
            }
            else if (equation == field::continuity_y)
            {
                 used_equation = field::velocity_y;
                 return;
            }
            else
            {
                used_equation = equation;
            }
            _equations->at(used_equation)->diffusive()->calculateOneCell(submatrix, cell->faces()[0], field, true);
            submatrix->addCellValues(cell, math::CellValue<double>(nullptr, 0));           
        }
    };
}
