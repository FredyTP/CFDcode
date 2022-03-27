/*****************************************************************//**
 * \file   cell_variable.h
 * \brief
 *
 *
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
namespace math
{
    class CellVariable
    {
    public:
        CellVariable(size_t _i_, size_t _j_, double _coef_) :i(_i_), j(_j_), coef(_coef_)
        {

        }


        size_t i; //Equation number
        size_t j; //cell variable 
        double coef; //Coeficient
    };
}
