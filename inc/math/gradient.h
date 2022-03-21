/*****************************************************************//**
 * \file   gradient.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <vector>

#include <Eigen/Eigen>

namespace mesh
{
    class Cell;
    class Face;
}


namespace math
{
    class GradientScheme
    {
    public:
        virtual double computeUnitaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face) const = 0;
        virtual std::vector<Eigen::Triplet<double>> computeFaceCoefs(const mesh::Cell* cell,const mesh::Face* face)const =0;
        virtual double computeUnitaryBoundaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face) const = 0;
    };

}
