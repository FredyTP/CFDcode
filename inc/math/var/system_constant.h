/*****************************************************************//**
 * \file   system_constant.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

namespace math
{
    class SystemConstant
    {
    public:
        SystemConstant(size_t _i_, double _coef_) :i(_i_), coef(_coef_)
        {

        }

        size_t i; //equation i
        double coef; //value

    };

}
