/*****************************************************************//**
 * \file   gradient_flux.h
 * \brief
 *
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

#include <math/value/face_value.h>
#include <math/var/system_submatrix.h>
#include <math/interpolation/gradient_flux.h>

#include <field/fields.h>

#include <terminal/named_object.h>
namespace term
{
    /**
     * Abstract class representing a gradient flux calculation method.
     */
    class GradientFlux : public terminal::NamedObject
    {
    public:
        GradientFlux(const std::string name) : NamedObject(name) {}
        /**
         * Pure virtual function representing how to interpolate the gradient on a face.
         *
         * \param cellvalues
         * \param coef
         * \param face
         */
        virtual void integrateGradientFace(std::vector<math::CellValue<double>>& cellvalues, double coef, const mesh::Face* face) = 0;

    };

    /**
     * Central Difference Gradient.
     * Aproximate the grandient flux by the partial derivative
     * in the direction of the vector that links both cells of the face.
     * It's a second order scheme but can lead to some error when the
     * mesh non-orthogonality is high.
     */
    class CentralDifferenceGradient : public GradientFlux
    {
    public:
        CentralDifferenceGradient() : GradientFlux("Central_Difference_Gradient")
        {

        }
        virtual void integrateGradientFace(std::vector<math::CellValue<double>>& cellvalues, double coef, const mesh::Face* face)
        {
            math::GradientFlux::SimpleGradientFlux(face, cellvalues, coef);
        }
            
    private:
        double computeGradient(const vector2d& pos1, const vector2d& pos2, const vector2d facenormal, double area)
        {
            vector2d r21 = (pos2 - pos1);
            double squaredDistance = r21.squaredNorm();
            vector2d unitgradient = r21 / squaredDistance;
            return (area * unitgradient.dot(facenormal));
        }
    };

    /**
     * Orthogonal Corrected Gradient.
     * Aproximate the grandient flux by the partial derivative
     * in the direction of the vector that links both cells of the face.
     * And in the perpendicular direction as the projection of the linear
     * interpolation both cell centroid gradients.
     * It's a second order scheme.
     */
    class OrthogonalCorrectedGradient : public GradientFlux
    {
    public:
        OrthogonalCorrectedGradient() : GradientFlux("Orthogonal_Corrected_Gradient")
        {

        }
        virtual void integrateGradientFace(std::vector<math::CellValue<double>>& cellvalues, double coef, const mesh::Face* face)
        {
            if (face->isBoundary())
            {
                math::GradientFlux::SimpleGradientFlux(face, cellvalues, coef);
            }
            else
            {
                math::GradientFlux::OrthogonalCorrection2(face, cellvalues, coef);

            }
        }

    private:
        double computeGradient(const vector2d& pos1, const vector2d& pos2, const vector2d facenormal, double area)
        {
            vector2d r21 = (pos2 - pos1);
            double squaredDistance = r21.squaredNorm();
            vector2d unitgradient = r21 / squaredDistance;
            return (area * unitgradient.dot(facenormal));
        }
    };

    
}




