/*****************************************************************//**
 * \file   gradient.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <math/central_difference_gradient.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

namespace math
{
    double CentralDifferenceGradient::computeUnitaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face) const
    {
        vector2d pos1 = cell->getCentroid();
        vector2d pos2 = face->getOtherCell(cell)->getCentroid();
        double modulo = (pos2 - pos1).norm();
        vector2d unitgradient = (pos2 - pos1) / pow(modulo, 2);

        return (face->area() * unitgradient.dot(face->getNormal(cell)));

    }
    double CentralDifferenceGradient::computeUnitaryBoundaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face) const
    {
        vector2d pos1 = cell->getCentroid();
        vector2d pos2 = face->getCentroid();
        double modulo = (pos2 - pos1).norm();
        vector2d unitgradient = (pos2 - pos1) / pow(modulo, 2);

        return (face->area() * unitgradient.dot(face->getNormal(cell)));
    }
    std::vector<Eigen::Triplet<double>> CentralDifferenceGradient::computeFaceCoefs(const mesh::Cell* cell, const mesh::Face* face) const
    {
        std::vector<Eigen::Triplet<double>> grad_coef;
        grad_coef.push_back(Eigen::Triplet<double>( cell->index(), cell->index(), +1));
        grad_coef.push_back(Eigen::Triplet<double>( cell->index(), face->getOtherCell(cell)->index(), -1 ));
        return grad_coef;
    }
    ;
}

