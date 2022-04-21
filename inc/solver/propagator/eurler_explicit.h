#pragma once
/*****************************************************************//**
 * \file   eurler_explicit.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <Eigen/Eigen>
#include <Eigen/SparseLU>
namespace solver
{
    /**
     *  Solve Equation in the form: M*phidot+F*phi=S
     * 
     * \param M
     * \param F
     * \param S
     * \param dt
     * \param lastStep
     * \param newStep
     */
    void EulerExplicit(const Eigen::SparseMatrix<double>& M, const Eigen::SparseMatrix<double>& F,const Eigen::VectorXd& S, double dt,
        const Eigen::VectorXd& lastStep, Eigen::VectorXd& newStep,bool simmetry = false)
    {
        using namespace Eigen;
        VectorXd ind = (M - F * dt) * lastStep + S * dt;
        if (simmetry)
        {
            SimplicialLLT<SparseMatrix<double>> linearSolver(M);
            newStep = linearSolver.solve(ind);
        }
        else
        {
            SparseLU<SparseMatrix<double>> linearSolver(M);
            newStep = linearSolver.solve(ind);
        }

    }
    void EulerImplicit(const Eigen::SparseMatrix<double>& M, const Eigen::SparseMatrix<double>& F, const Eigen::VectorXd& S, double dt,
    const Eigen::VectorXd& lastStep, Eigen::VectorXd& newStep)
    {
        using namespace Eigen;
        SparseMatrix<double> dep = M + F * dt;
        VectorXd ind = M * lastStep + S * dt;

        SparseLU<SparseMatrix<double>> linearSolver(dep);
        newStep = linearSolver.solve(ind);
        

    }
}