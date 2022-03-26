/*****************************************************************//**
 * \file   system_submatrix.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once


//STL
#include <vector>

//CFD
#include <math/var/cell_variable.h>
#include <math/var/system_constant.h>



namespace math
{
    class SystemSubmatrix
    {
    public:
        SystemSubmatrix()
        {

        }
        void addCellVar(CellVariable cellvar)
        {
            cellVars.push_back(cellvar);
        }
        void addConstant(SystemConstant constant)
        {
            constants.push_back(constant);
        }
        void merge(SystemSubmatrix& other)
        {
            for (auto cellvar : other.cellVars)
            {
                this->addCellVar(cellvar);
            }
            for (auto sysconst : other.constants)
            {
                this->addConstant(sysconst);
            }
        }
        void merge(std::vector<SystemSubmatrix>& other)
        {
            for (auto submatrix : other)
            {
                this->merge(submatrix);
            }
        }
        std::vector<Eigen::Triplet<double>> toTriplets()
        {
            std::vector<Eigen::Triplet<double>> triplets;
            triplets.reserve(cellVars.size());
            for (auto & var : cellVars)
            {
                triplets.push_back(Eigen::Triplet<double>( var.i, var.j, var.coef));
            }
            return triplets;
        }
        Eigen::VectorXd toVector(size_t size)
        {
            Eigen::VectorXd vec(size);
            vec.setZero();
            for(auto con : constants)
            {
                vec(con.i) += con.coef;
            }
            return vec;
        }

        std::vector<CellVariable> cellVars;
        std::vector<SystemConstant> constants;
    };
}
