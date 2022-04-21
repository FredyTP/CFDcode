#pragma once
/*****************************************************************//**
 * \file   source_term.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <mesh/base/face.h>
#include <math/var/system_submatrix.h>
#include <field/field.h>
#include <mesh/manipulation/mesh_selection.h>

namespace term
{
    class SourceTerm : public CellEquationTerm
    {
    public:
        SourceTerm(double _value_) : _value(_value_) {}
        void setCells(const mesh::MeshSelection<mesh::Cell>& selection)
        {
            selection.copyInto(_cells);
        }
        void calculate(math::SystemSubmatrix* submatrix, const field::Fields* fields)
        {
            for (auto cell : _cells)
            {
                this->calculateCell(submatrix, cell, fields);
            }
        }
        void calculateCell(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* fields)
        {
            double coef = _value * cell->volume();
            submatrix->addCellValues(cell, math::CellValue<double>(nullptr, coef));
        }
    private:
        double _value;
        std::vector<mesh::Cell*> _cells;
    };
}