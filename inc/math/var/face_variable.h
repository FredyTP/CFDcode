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
        FaceVariable(vector2d _pos_, const mesh::Face* _face_, const mesh::Cell* _cell_, double _coef_)
            :pos(_pos_), face(_face_), cell(_cell_), coef(_coef_)
        {

        }

        vector2d pos; //Where is the variable located in the face
        const mesh::Face* face; //In which face in located the variable
        const mesh::Cell* cell; //here we know eq number i = cell->index();

        double coef;

    };
}
