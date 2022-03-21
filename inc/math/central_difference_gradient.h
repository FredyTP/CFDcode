/*****************************************************************//**
 * \file   gradient.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once 

#include <vector>
#include <math/gradient.h>
#include <Eigen/Eigen>


namespace math
{

    class CentralDifferenceGradient : public GradientScheme
    {
    public:
        CentralDifferenceGradient() {}

        virtual double computeUnitaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face)const ;
        virtual std::vector<Eigen::Triplet<double>> computeFaceCoefs(const mesh::Cell* cell, const mesh::Face* face)const ;
        virtual double computeUnitaryBoundaryFaceFlux(const mesh::Cell* cell, const mesh::Face* face)const ;
        

    };


}
