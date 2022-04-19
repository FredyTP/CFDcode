#pragma once
/*****************************************************************//**
 * \file   face_value.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <mesh/base/face.h>
namespace math
{
    template<typename _Ty>
    class FaceValue
    {
    public:
        FaceValue(mesh::Face* _face_, _Ty _coef_) : face(_face_), coef(_coef_)
        {

        }
        mesh::Face* face;
        _Ty coef;

    };
}
