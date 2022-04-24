#pragma once
/*****************************************************************//**
 * \file   time_step_activity.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#include <base/global_typedef.h>
#include <system/problem.h>

namespace solver
{
    class TimeStepActivity
    {
    public:
        TimeStepActivity(){}
        void setAction(std::function<void(double, double, sys::Problem*, field::Fields*)> action)
        {
            _action = action;
        }
        virtual void timeStepAction(double actualTime, double time_step, sys::Problem* problem, field::Fields* solution) = 0;
    protected:
        void execute(double actualTime, double time_step, sys::Problem* problem, field::Fields* solution)
        {
            _action(actualTime, time_step, problem, solution);
        }
    private:
        std::function<void(double, double, sys::Problem*, field::Fields*)> _action;
    };
    
    class FixedStepActivity : public TimeStepActivity
    {
    public:
        FixedStepActivity(uint32_t _steps_) : _steps(_steps_) {}

        virtual void timeStepAction(double actualTime, double time_step, sys::Problem* problem, field::Fields* solution)
        {
            _stepCounter++;
            _totalSteps++;
            if (_stepCounter >= _steps)
            {
                _stepCounter = 0;
                TimeStepActivity::execute(actualTime, time_step, problem, solution);
            }
        }
        uint32_t step() const { return _totalSteps; }
    private:
        uint32_t _steps;
        uint32_t _stepCounter = 0;
        uint32_t _totalSteps = 0;
    };

    class FixedTimeActivity : public TimeStepActivity
    {
    public:
        FixedTimeActivity(double _interval_) : _interval(_interval_) {}

        virtual void timeStepAction(double actualTime, double time_step, sys::Problem* problem, field::Fields* solution)
        {
            _deltaTime += time_step;
            if (_deltaTime >= _interval)
            {
                _deltaTime = 0;
                TimeStepActivity::execute(actualTime, time_step, problem, solution);
            }
        }
    private:
        double _interval;
        double _deltaTime = 0.0;
    };

}
