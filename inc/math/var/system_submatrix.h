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
        SystemSubmatrix(int size)
        {
            constants.resize(size);
            constants.setZero();
        }
        void addCellVar(CellVariable cellvar)
        {
            cellVars.push_back(Eigen::Triplet<double>(cellvar.i,cellvar.j,cellvar.coef));
        }
        void addConstant(SystemConstant constant)
        {
            constants(constant.i) += constant.coef;
        }
        /*void merge(SystemSubmatrix& other)
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
        }*/
        std::vector<Eigen::Triplet<double>> toTriplets()
        {
            return cellVars;
            /*
            std::vector<Eigen::Triplet<double>> triplets;
            triplets.reserve(cellVars.size());
            for (auto& var : cellVars)
            {
                triplets.push_back(Eigen::Triplet<double>(var.i, var.j, var.coef));
            }
            return triplets;*/
        }
        Eigen::VectorXd toVector()
        {
            return constants;
            /*Eigen::VectorXd vec(size);
            vec.setZero();
            for (auto con : constants)
            {
                vec(con.i) += con.coef;
            }
            return vec;*/
        }
    private:
        std::vector<Eigen::Triplet<double>> cellVars;
        Eigen::VectorXd constants;
    };
}
