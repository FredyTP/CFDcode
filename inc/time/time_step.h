/*****************************************************************//**
 * \file   time_step.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <field/field.h>

namespace timestep
{
    /**
     * \brief Class for time management in termporal solvers.
     * Derived class must be provided implementing how the time step
     * is computed. Fixed Time step is provided. Time is stored in seconds.
     */
    class TimeStep
    {
    public:
        TimeStep(){}

        //GETTERS 
        virtual double currentTime() const { return _currentTime; }
        virtual double timeStep() const { return _timeStep;  }

        //Time Steps
        virtual void nextTimeStep(const field::Fields* currentSolution)
        {
            updateTimeStep(currentSolution);
            _currentTime += _timeStep;
        }
        //Reset internal time
        void resetTime()
        {
            _currentTime = 0.0;
        }
    protected:
        virtual void updateTimeStep(const field::Fields* currentSolution) = 0;
        double _timeStep = 0.0;

    private:
        double _currentTime = 0.0;
    };

    class FixedTimeStep : public TimeStep
    {
    public:
        FixedTimeStep(double _timeStep_){_timeStep = _timeStep_; }
        virtual void updateTimeStep(const field::Fields* currentSolution){}
         
    };

}
