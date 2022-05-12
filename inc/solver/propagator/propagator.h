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
#include <terminal/named_object.h>
namespace solver
{

    class Propagator : public terminal::NamedObject
    {

    public:
        Propagator(const std::string name) : NamedObject(name) {}
        virtual void propagate(const Eigen::SparseMatrix<double>& M, const Eigen::SparseMatrix<double>& F, const Eigen::VectorXd& S, double dt,
    const Eigen::VectorXd& lastStep, Eigen::VectorXd& newStep) = 0;
    };


    /**
     * Implementation of Euler Implicit propagator method.
     */
    class EulerImplicit : public Propagator
    {
    public:
        EulerImplicit() : Propagator("Euler_Implicit") {}
        /**
        *  Solve Equation in the form: M*phidot+F*phi=S with Euler Implicit method
        *
        * \param M phi dot coeficients
        * \param F phi coeficients
        * \param S independent vector
        * \param dt delta time
        * \param lastStep last step results
        * \param newStep new step results
        */
        virtual void propagate(const Eigen::SparseMatrix<double>& M, const Eigen::SparseMatrix<double>& F, const Eigen::VectorXd& S, double dt,
    const Eigen::VectorXd& lastStep, Eigen::VectorXd& newStep)
        {
            using namespace Eigen;
            SparseMatrix<double> dep = M + F * dt;
            VectorXd ind = M * lastStep + S * dt;

            SparseLU<SparseMatrix<double>> linearSolver(dep);
            newStep = linearSolver.solve(ind);
        }
    };

    /**
     * Implementation of Crank-Nicolson propagator method.
     */
    class CrankNicolson : public Propagator
    {
    public:
        CrankNicolson() : Propagator("Crank_Nicolson") {}
        /**
         *  Solve Equation in the form: M*phidot+F*phi=S with Euler Implicit method
         *
         * \param M phi dot coeficients
         * \param F phi coeficients
         * \param S independent vector
         * \param dt delta time
         * \param lastStep last step results
         * \param newStep new step results
         */
        virtual void propagate(const Eigen::SparseMatrix<double>& M, const Eigen::SparseMatrix<double>& F, const Eigen::VectorXd& S, double dt,
    const Eigen::VectorXd& lastStep, Eigen::VectorXd& newStep)
        {
            using namespace Eigen;
            SparseMatrix<double> dep = M + F * 0.5 * dt;
            VectorXd ind = (M - F * 0.5 * dt) * lastStep + S * dt;

            SparseLU<SparseMatrix<double>> linearSolver(dep);
            newStep = linearSolver.solve(ind);
        }
    };

}