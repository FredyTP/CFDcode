#pragma once

/*****************************************************************//**
 * \file   gradient_interpolation.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <vector>
#include <base/global_typedef.h>

#include <math/value/cell_value.h>
#include <math/value/face_value.h>
#include <math/value/node_value.h>

namespace math
{
    /**
     * \brief Interpolates gradient center cell value based on cell values.
     */
    class GradientInterpolation
    {
    public:
        static void GreenGaussGradient(const mesh::Cell* cell, std::vector<FaceValue<vector2d>>& facevalues)
        {
            double volume = cell->volume();
            for (auto face : cell->faces())
            {
                facevalues.push_back(FaceValue<vector2d>(face, face->area() / volume * face->getNormal(cell)));
            }
        }

        //Least Squares Gradient
        static void LeastSquaresGradient(const mesh::Cell* cell, std::vector<FaceValue<vector2d>>& facevalues)
        {
            //TODO: implement
        }
    };
}
