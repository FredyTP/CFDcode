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

#include <math/var/face_variable.h>
#include <math/var/system_submatrix.h>

#include <field/field.h>
namespace math
{
    namespace diffusive
    {
        class GradientFlux
        {
        public:
            GradientFlux() {}
            virtual void integrateGradient(SystemSubmatrix* submatrix, double coef, const mesh::Cell* cell, const mesh::Face* face) = 0;
            virtual void integrateGradientBoundary(SystemSubmatrix* submatrix, double coef, const mesh::Cell* cell,
                double boundaryValue, const mesh::Face* face) = 0;
        };

        class CentralDifferenceGradient : public GradientFlux
        {
        public:
            CentralDifferenceGradient()
            {

            }
            virtual void integrateGradient(SystemSubmatrix* submatrix, double coef, const mesh::Cell* cell, const mesh::Face* face)
            {
                double gradient = computeGradient(cell->getCentroid(), face->getOtherCell(cell)->getCentroid(), 
                    face->getNormal(cell), face->area());

                submatrix->addCellVar(CellVariable(cell->index(), cell->index(), gradient*coef));
                submatrix->addCellVar(CellVariable(cell->index(), face->getOtherCell(cell)->index(), -gradient*coef));      
              
            }
            virtual void integrateGradientBoundary(SystemSubmatrix* submatrix, double coef, const mesh::Cell* cell,double boundaryValue, const mesh::Face* face)
            {
                
                double gradient = computeGradient(cell->getCentroid(), face->getCentroid(),
                    face->getNormal(cell), face->area());

                submatrix->addCellVar(CellVariable(cell->index(), cell->index(), gradient * coef));
                submatrix->addConstant(SystemConstant(cell->index(), gradient * boundaryValue * coef));

            }
        private:
            double computeGradient(const vector2d& pos1, const vector2d& pos2, const vector2d facenormal,double area)
            {
                vector2d r21 = (pos2 - pos1);
                double squaredDistance = r21.squaredNorm();
                vector2d unitgradient = r21 / squaredDistance;
                return (area * unitgradient.dot(facenormal));
            }
        };

    }
}




