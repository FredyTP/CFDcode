/*****************************************************************//**
 * \file   interpolated_variable.h
 * \brief
 *
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <base/global_typedef.h>
#include <mesh/base/face.h>

namespace math
{
    class FaceVariable
    {
    public:
        FaceVariable(vector2d _pos_, const mesh::Face* _face_, const size_t _eq_, double _coef_)
            :pos(_pos_), face(_face_), eq(_eq_), coef(_coef_)
        {

        }

        vector2d pos; //Where is the variable located in the face
        const mesh::Face* face; //In which face in located the variable
        size_t eq; //here we know eq number i = cell->index();

        double coef;

    };
}
