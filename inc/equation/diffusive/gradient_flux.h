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
namespace term
{
    class GradientFlux
    {
    public:
        GradientFlux() {}
        virtual void integrateGradientFace(std::vector<math::CellValue<double>>& cellvalues, double coef, const mesh::Face* face) = 0;

    };

    class CentralDifferenceGradient : public GradientFlux
    {
    public:
        CentralDifferenceGradient()
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

    class OrthogonalCorrectedGradient : public GradientFlux
    {
    public:
        OrthogonalCorrectedGradient()
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
                math::GradientFlux::OrthogonalCorrectedFlux(face, cellvalues, coef);

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




