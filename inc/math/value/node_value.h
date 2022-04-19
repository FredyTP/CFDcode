#pragma once
/*****************************************************************//**
 * \file   node_value.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <mesh/base/node.h>
namespace math
{
    template<typename _Ty>
    class NodeValue
    {
    public:
        NodeValue(mesh::Node* _node_, _Ty _coef_) : node(_node_), coef(_coef_)
        {

        }
        mesh::Node* node;
        _Ty coef;

    };
}
