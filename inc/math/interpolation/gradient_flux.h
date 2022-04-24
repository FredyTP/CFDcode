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
            std::vector<CellValue<vector2d>> gradient;
            GradientInterpolation::FiniteDifferenceGradient(face, gradient);
            //TODO: separar cosas que calculan valores de las que calculan ecuaciones
            //flux se calcularia en una ecuacion separada
            vector2d Sf = factor * face->area() * face->normal1();
            GradientInterpolation::DotProduct(cellvalues, gradient, Sf);
 
        }
        static void OrthogonalCorrectedFlux(const mesh::Face* face, std::vector<CellValue<double>>& cellvalues, double factor = 1.0f)
        {
            vector2d df = (face->cell2()->getCentroid() - face->cell1()->getCentroid());
            //vector2d Deltaf = face->area() * df / (df.dot(face->normal1()));
            vector2d Sf = face->area() * face->normal1();
            vector2d Deltaf = (Sf.dot(Sf) / df.dot(Sf)) * df;
            vector2d Kf = face->area() * face->normal1() - Deltaf;

            double flux1 = factor * Deltaf.norm()/df.norm();
            vector2d flux2 = factor * Kf;

            //std::vector<CellValue<vector2d>> gradient1;
            //GradientInterpolation::FiniteDifferenceGradient(face, gradient1);
            cellvalues.push_back(CellValue<double>(face->cell2(), flux1));
            cellvalues.push_back(CellValue<double>(face->cell1(), -flux1));
            std::vector<CellValue<vector2d>> gradient2;
            GradientInterpolation::GreenGaussGradient(face->cell1(), gradient2, 0.5);
            GradientInterpolation::GreenGaussGradient(face->cell2(), gradient2, 0.5);

            //GradientInterpolation::DotProduct(cellvalues, gradient1, flux1);
            GradientInterpolation::DotProduct(cellvalues, gradient2, flux2);


        }
        static void TestFlux(const mesh::Face* face, std::vector<CellValue<double>>& cellvalues, double factor = 1.0f)
        {
            vector2d df = (face->cell2()->getCentroid() - face->cell1()->getCentroid());
            vector2d sf = face->area()*face->normal1();

           

            vector2d dir = df/df.norm();
            vector2d norm(dir.y(), -dir.x());

            std::vector<CellValue<vector2d>> gradient;
            GradientInterpolation::FiniteDifferenceGradient(face, gradient);
            //TODO: separar cosas que calculan valores de las que calculan ecuaciones
            //flux se calcularia en una ecuacion separada
            vector2d Sf = factor * face->area() * face->normal1();
            GradientInterpolation::DotProduct(cellvalues, gradient, Sf);

            std::vector<CellValue<vector2d>> gradient2;

            GradientInterpolation::GreenGaussGradient(face->cell1(), gradient2,0.5);
            GradientInterpolation::GreenGaussGradient(face->cell2(), gradient2, 0.5);

            vector2d grad2_vec = factor * face->area() * norm.dot(face->normal1()) * norm;
            GradientInterpolation::DotProduct(cellvalues, gradient2, grad2_vec);

            
           
   

        }


        // add gradient flux with non-orthogonal correction
    };
}
