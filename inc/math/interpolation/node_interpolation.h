/*****************************************************************//**
 * \file   node_interpolation.h
 * \brief  
 * 
 * \author alfre
 * \date   May 2022
 *********************************************************************/
#pragma once
#include <math/value/cell_value.h>
#include <math/value/node_value.h>
namespace math
{
    class NodeInterpolation
    {
    public:
        /**
         * \brief Computes the Central Differencing Scheme (CDS) interpolation
         * on the cell's face.
         *
         * \param lamba relation of cell distance to face divided to distance
         * of centroid's cells connected bu the face
         * \param phi_cell value of phi of the cell
         * \param phi_neig value of phi of neightbour cell
         * \return
         */
            template<class _Type>
        static void CellMeanInterpolation(const NodeValue<_Type>& nodevalue, std::vector<CellValue<_Type>>& cellvalues, _Type factor)
        {
            int size = nodevalue.node->cells().size();
            for (auto cell : nodevalue.node->cells())
            {
                cellvalues.push_back(CellValue<_Type>(cell, nodevalue.coef * factor / size));
            }
        }
    };
}
