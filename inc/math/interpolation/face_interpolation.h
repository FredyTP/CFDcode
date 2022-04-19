#pragma once

/*****************************************************************//**
 * \file   face_interpolation.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#include <vector>
#include <base/global_typedef.h>

#include <math/value/cell_value.h>
#include <math/value/face_value.h>
#include <math/value/node_value.h>

#include <math/interpolation/gradient_interpolation.h>

namespace math
{
 /**
 * Class containing different interpolation schemes to interpolate
 * field values in the cell face
 */
    class FaceInterpolation
    {
    public:
        /**
         * \brief Computes the Upwind Differencing Scheme(UDS) interpolation
         * on the cell's face.
         *
         * Properties:
         * - First order
         * - Unconditionallly stable(no oscillations)
         * - Numerically diffusive
         *
         * \param vel_dot_normal: Dot product of vel on face with normal vector to the face (pointing outside the cell)
         * \param cell_coef: Value at the cell center
         * \param cell_neig_coef: Value at the cell neighbour connected by the face
         */
        template<class _Type>
        static void UpwindDifferencingScheme(const mesh::Face* face,
            const double vel_dot_normal, std::vector<CellValue<_Type>>& cellvalues, double factor = 1.0)
        {

           cellvalues.push_back(CellValue<_Type>(_GetUpstreamCell(face,vel_dot_normal), 1 * factor));

        }

        /**
         * \brief Computes the Central Differencing Scheme (CDS) interpolation
         * on the cell's face.
         *
         * \param lamba relation of cell distance to face divided to distance
         * of centroid's cells connected bu the face
         * \param phi_cell value of phi of the cell
         * \param phi_neig value of phi of neightbour cell
         * \return
         */
        template<class _Type>
        static void CentralDifferencingScheme(const mesh::Face* face,
            const double lambda, std::vector<CellValue<_Type>>& cellvalues, double factor = 1.0)
        {
            cellvalues.push_back(CellValue<_Type>(face->cell1(), (1 - lambda) * factor));
            cellvalues.push_back(CellValue<_Type>(face->cell2(), lambda * factor));
        }

        /**
         * .
         *
         * \param cell
         * \param face
         * \param peclet_number
         * \param lambda
         * \param cellvalues
         */
        template<class _Type>
        static void PowerLaw(const mesh::Face* face,
            const double peclet_number, const double lambda, std::vector<CellValue<_Type>>& cellvalues, double factor = 1.0)
        {
            double peclet_abs = abs(peclet_number);
            if (isinf(peclet_number) || isnan(peclet_number))
            {
                FaceInterpolation::UpwindDifferencingScheme(face, peclet_number, cellvalues, factor);
            }
            else if (peclet_abs < 1e-5)
            {
                FaceInterpolation::CentralDifferencingScheme(face, lambda, cellvalues, factor);
            }
            else
            {
                double peclet_val = (exp(peclet_number * lambda) - 1) / (exp(peclet_number) - 1);
                FaceInterpolation::CentralDifferencingScheme(face, peclet_val, cellvalues, factor);
            }
        }


        static void SecondOrderUpwind(const mesh::Cell* cell, const mesh::Face* face,
            double vel_dot_normal, std::vector<CellValue<double>>& cellvalues, double factor = 1.0)
        {
                       
            if (face->isBoundary())
            {
                UpwindDifferencingScheme<double>(face, vel_dot_normal, cellvalues, factor);
            }
            else
            {
                //Provide this as parameter :)
                std::vector<FaceValue<vector2d>> facevalues;
                const mesh::Cell* upstream_cell = _GetUpstreamCell(face, vel_dot_normal);
                GradientInterpolation::GreenGaussGradient(upstream_cell, facevalues);

                CellValue<double> center_cell(upstream_cell, factor);
                cellvalues.push_back(center_cell);
                vector2d celltoface = (face->getCentroid() - upstream_cell->getCentroid());
                for (int i = 0; i < facevalues.size(); i++)
                {
                    double dotprod = celltoface.dot(facevalues[i].coef);
                    FaceInterpolation::CentralDifferencingScheme(facevalues[i].face, face->lambda(), cellvalues, dotprod * factor);
                   
                }

            }

        }
        //UNIMPLEMENTED INTERPOLATION METHODS
        //template<class type> static type quick()=0;
        //template<class type> static type power_law() = 0;
        private:
            //HELPER FUNCTIONS
            static mesh::Cell* _GetUpstreamCell(const mesh::Face* face, double vel_dot_normal)
            {
                if (vel_dot_normal > 0)
                {
                    return face->cell1();
                }
                else
                {
                    return face->cell2();
                }
            }

    };
}
