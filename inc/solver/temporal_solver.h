#pragma once
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
#include <solver/time/time_step.h>
#include <solver/propagator/eurler_explicit.h>
#include <solver/activity/time_step_activity.h>
#include <solver/stop/stopping_criteria.h>

namespace solver
{
    class TemporalSolver
    {
    public:
        TemporalSolver(MatrixBuilder* _builder_) : _builder(_builder_)
        {

        }
        void solve(sys::Problem* problem,timestep::TimeStep* timeStep,stop::StoppingCriteria* stoppingCriteria)
        {
            //RESET TIMER
            timeStep->resetTime();

            //CREATE NEW SOLUTION
            _solution = std::make_unique<field::Fields>(problem->mesh());

            //SET INITIAL CONDITIONS
            _solution->copy(problem->fields());
            
            //CalculateMATRIX
            _builder->buildSystem(problem,field::temperature);
            Eigen::SparseMatrix<double> &matrix = _builder->getMatrix();
            Eigen::VectorXd &indep = _builder->getVector();
            Eigen::SparseMatrix<double> &volumeMatrix = _builder->getVolMatrix();
                  
            double dt = 0.0; 
            timeStep->nextTimeStep(_solution.get());
          
            while ( stoppingCriteria->shouldContinue(timeStep->currentTime(),problem,_solution.get()) )
            {
                dt = timeStep->timeStep();
                Eigen::VectorXd newSolution;
                EulerImplicit(volumeMatrix, matrix, indep, dt, _solution->scalarField(field::temperature), newSolution);
                _solution->scalarField(field::temperature) = newSolution;
                for (auto activity : _timeStepActivities)
                {
                    activity->timeStepAction(timeStep->currentTime(), dt, problem, _solution.get());
                }
                timeStep->nextTimeStep(_solution.get());        
            }

            //save_contour(savefile, imgfile ,1920,1080);           
        }

        void addActivity(TimeStepActivity* activity)
        {
            _timeStepActivities.push_back(activity);
        }



        

    private:
        MatrixBuilder* _builder;
        std::unique_ptr<field::Fields> _solution;
        std::vector<TimeStepActivity*> _timeStepActivities;
    };
}
