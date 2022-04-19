#pragma once
/*****************************************************************//**
 * \file   cell_value.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#include <mesh/base/cell.h>
namespace math
{
    template<typename _Ty>
    class CellValue
    {
    public:
        CellValue(const mesh::Cell* _cell_, _Ty _coef_) : cell(_cell_), coef(_coef_)
        {

        }
        CellValue<_Ty>& invert() {
            coef *= -1;
            return *this;
        }
        const mesh::Cell* cell;
        _Ty coef;

    };
}
