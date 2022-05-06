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
#include <solver/matrix/matrix_builder.h>
#include <solver/time/time_step.h>
#include <solver/propagator/propagator.h>
#include <solver/activity/time_step_activity.h>
#include <solver/stop/stopping_criteria.h>

namespace solver
{

    /**
     * Solves the time evolution case.
     */
    class TemporalSolver
    {
    public:
        TemporalSolver(BaseMatrixBuilder* _builder_) : _builder(_builder_)
        {

        }
        void solve(sys::Problem* problem,timestep::TimeStep* timeStep,stop::StoppingCriteria* stoppingCriteria,solver::Propagator* propagator)
        {
            //RESET TIMER
            timeStep->resetTime();

            //CREATE NEW SOLUTION
            _solution = std::make_unique<field::Fields>(problem->mesh());

            //SET INITIAL CONDITIONS
            _solution->copy(problem->fields());
            

                  
            double dt = 0.0; 
            timeStep->nextTimeStep(_solution.get());
          
            while ( stoppingCriteria->shouldContinue(timeStep->currentTime(),problem,_solution.get()) )
            {
                dt = timeStep->timeStep();
                //CalculateMATRIX
                _builder->buildSystem(problem);
                Eigen::SparseMatrix<double>& matrix = _builder->getMatrix();
                Eigen::VectorXd& indep = _builder->getVector();
                Eigen::SparseMatrix<double>& volumeMatrix = _builder->getVolMatrix();
                Eigen::VectorXd newSolution;
                propagator->propagate(volumeMatrix, matrix, indep, dt, _solution->scalarField(field::temperature), _solution->scalarField(field::temperature));
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



        
        field::Fields* solution() { return _solution.get(); }

    private:
        BaseMatrixBuilder* _builder;
        std::unique_ptr<field::Fields> _solution;
        std::vector<TimeStepActivity*> _timeStepActivities;
    };
}
