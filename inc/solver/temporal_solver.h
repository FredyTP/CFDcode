/*****************************************************************//**
 * \file   temporal_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <string>

//EIGEN
#include <Eigen/SparseLU>

//CFD
#include <solver/matrix_builder.h>
#include <time/time_step.h>
#include <solver/propagator/eurler_explicit.h>

namespace solver
{
    class TemporalSolver
    {
    public:
        TemporalSolver(MatrixBuilder* _builder_) : _builder(_builder_)
        {

        }
        void solve(const mesh::Mesh* pMesh,
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective,
        field::Fields* initialCondition)
        {
            _solution = std::make_unique<field::Fields>(pMesh);
            _solution->copy(initialCondition);
            _builder->buildSystem(pMesh, _bConditions, pDiffusive, pConvective, _solution.get());
            Eigen::SparseMatrix<double> &matrix = _builder->getMatrix();
            Eigen::VectorXd &indep = _builder->getVector();
            Eigen::SparseMatrix<double> &volumeMatrix = _builder->getVolMatrix();
            
            size_t size = pMesh->cells()->size();

            
            timestep::FixedTimeStep timeStep(0.0003);
            double dt = timeStep.timeStep();
            //FORWARD NEWTON
            std::string savefile = "temp_int_file.txt";
            std::string imgfile = "temp_img_UDS" + std::to_string(_solution->rawVelocity()[0].y()) + ".jpg";
            initfile(savefile);
            save(savefile, timeStep.currentTime(), _solution.get(), pMesh);
            while (timeStep.currentTime() < 10 * dt)
            {
                dt = timeStep.timeStep();
                /*Eigen::SparseMatrix<double> coefMatrix = (volumeMatrix + matrix * dt);
                Eigen::VectorXd vector = dt * indep +(volumeMatrix*_solution->rawTemperature());
         
                Eigen::SparseLU<Eigen::SparseMatrix<double>> chol(coefMatrix);
                _solution->rawTemperature() = chol.solve(vector);*/
                Eigen::VectorXd newSolution;
                EulerImplicit(volumeMatrix, matrix, indep, dt, _solution->rawTemperature(), newSolution);
                timeStep.nextTimeStep(_solution.get());
                _solution->rawTemperature() = newSolution;
                save(savefile, timeStep.currentTime(), _solution.get(), pMesh);
            }

            save_contour(savefile, imgfile ,1920,1080);

            
        }
        void initfile(const std::string& filename)
        {
            std::ofstream file;
            file.open(filename, std::ios::trunc);
            if (file.is_open())
            {
                file << "X" << "," << "Y" << "," << "Value" << "," << "VX1" << "," << "VY1" << "," << "VX2" << "," << "VY2" << "," << "VX3" << "," << "VY3" << std::endl;
                file.close();
            }
        }
        void save(const std::string& filename, double time, field::Fields* _solution, const mesh::Mesh* pMesh)
        {
            std::ofstream file;
            file.open(filename, std::ios::app);
            if (file.is_open())
            {
                double deltax = static_cast<double>(100)/3.0 * time;
                for (int i = 0; i < _solution->temperature().size(); i++)
                {
                    auto cell = pMesh->cells()->at(i).get();
                    auto n0 = cell->nodes().at(0);
                    auto n1 = cell->nodes().at(1);
                    auto n2 = cell->nodes().at(2);

                    vector2d pos = pMesh->cells()->at(i)->getCentroid();

                    file << pos.x() + deltax<< "," << pos.y() << "," << _solution->rawTemperature()(i)
                        << "," << n0->pos().x() + deltax << "," << n1->pos().x() + deltax
                        << "," << n2->pos().x() + deltax << "," << n0->pos().y()
                        << "," << n1->pos().y() << "," << n2->pos().y() << std::endl;
                }
                file.close();
            }
        }
        void save_contour(const std::string& filename, const std::string& contour_filename, size_t Rx, size_t Ry)
        {
            std::cout << "Saving: " << filename << " to: "<<contour_filename <<" ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('"+filename+"','"+contour_filename+"',"+std::to_string(Rx)+","+ std::to_string(Ry)+")\"";
            system(command.c_str());
            std::cout << "saved" << std::endl;
            std::string openimg = "\"" + contour_filename + "\"";
            system(openimg.c_str());
        }
    private:
        MatrixBuilder* _builder;
        std::unique_ptr<field::Fields> _solution;
    };
}
