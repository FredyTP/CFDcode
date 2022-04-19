/*****************************************************************//**
 * \file   gradient_flux.h
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

#include <mesh/base/face.h>

namespace math
{
    class GradientFlux
    {
    public:
        /**
         * .
         *
         * \param pos1
         * \param pos2
         * \param facenormal
         * \param area
         * \param cell_coef
         * \param cell_neig_coef
         */
        static void SimpleGradientFlux(const mesh::Face* face, std::vector<CellValue<double>>& cellvalues, double factor = 1.0f)
        {
            vector2d r21 = (face->cell2()->getCentroid() - face->cell1()->getCentroid());
            //TODO: separar cosas que calculan valores de las que calculan ecuaciones
            //flux se calcularia en una ecuacion separada
            double flux = face->area() * r21.dot(face->normal1()) / face->distanceCell2Cell();
            cellvalues.push_back(CellValue<double>(face->cell1(), flux * factor));
            cellvalues.push_back(CellValue<double>(face->cell2(), -flux * factor));
            
        }


        // add gradient flux with non-orthogonal correction
    };
}
