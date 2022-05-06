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
        * Interpolates a nodal value base on the mean of the surrounding cells values.
        * 
        * \param nodevalue
        * \param cellvalues
        * \param factor
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
