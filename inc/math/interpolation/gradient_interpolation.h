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


        static void FiniteDifferenceGradient(const mesh::Face* face, std::vector<CellValue<vector2d>>& cellvalues, double factor = 1.0);

        //Green-Gauss Gradient interpolation
        static void GreenGaussGradient(const mesh::Cell* cell, std::vector<CellValue<vector2d>>& cellvalues, double factor = 1.0);


        //Least Squares Gradient
        static void LeastSquaresGradient(const mesh::Cell* cell, std::vector<CellValue<vector2d>>& cellvalues, double factor = 1.0);

        static void DotProduct(std::vector<CellValue<double>>& result, std::vector<CellValue<vector2d>>& cellvalues, vector2d vec);
    };

}
