#pragma once
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
#include <math/interpolation/node_interpolation.h>


namespace math
{
    class GradientFlux
    {
    public:
        /**
         * Computes simple central differencing gradient
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
            std::vector<CellValue<vector2d>> gradient;
            GradientInterpolation::FiniteDifferenceGradient(face, gradient);
            //TODO: separar cosas que calculan valores de las que calculan ecuaciones
            //flux se calcularia en una ecuacion separada
            vector2d Sf = factor * face->area() * face->normal1();
            GradientInterpolation::DotProduct(cellvalues, gradient, Sf);
 
        }
        
        /**
         * Computes the orthogonal corrected gradient flux on selected face.
         * 
         * \param face
         * \param cellvalues
         * \param factor
         */
        static void OrthogonalCorrection(const mesh::Face* face, std::vector<CellValue<double>>& cellvalues, double factor = 1.0f)
        {
            vector2d n = face->normal1();
            vector2d eta = face->node2()->position() - face->node1()->position();
            vector2d xi = face->cell2()->position() - face->cell1()->position();
            vector2d e_eta = eta.normalized();
            vector2d e_xi = (xi).normalized();

            double coef_xi = face->area() / (xi.norm() * face->normal1().dot(e_xi));
            double coef_eta = -face->area()*(e_xi.dot(e_eta)) / (eta.norm() * n.dot(e_xi));

            cellvalues.push_back(CellValue<double>(face->cell2(), factor * coef_xi));
            cellvalues.push_back(CellValue<double>(face->cell1(), -factor * coef_xi));


            NodeValue<double> node2(face->node2(), coef_eta);
            NodeValue<double> node1(face->node1(), -coef_eta);
            NodeInterpolation::CellMeanInterpolation(node2, cellvalues, factor);
            NodeInterpolation::CellMeanInterpolation(node1, cellvalues, factor);           
        }

        
    };
}
