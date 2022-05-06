/*****************************************************************//**
 * \file   stopping_criteria.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#pragma once

#include <base/global_typedef.h>
#include <system/problem.h>

namespace solver
{
    namespace stop
    {
        /**
         * Class abstraction of the stopping criteria.
         */
        class StoppingCriteria
        {
        public:
            StoppingCriteria() {}

            /**
             * Return if should continue iterating.
             * 
             * \param actualTime
             * \param problem
             * \param solution
             * \return 
             */
            virtual bool shouldContinue(double actualTime, sys::Problem* problem, field::Fields* solution) = 0;
        };

        /**
         * Stops after the specified number of steps.
         */
        class StopAtStep : public StoppingCriteria
        {

        public:
            StopAtStep(uint32_t _time_steps_)
            {
                _time_steps = _time_steps_;
            }
            virtual bool shouldContinue(double actualTime, sys::Problem* problem, field::Fields* solution)
            {
                _step_counter++;
                return (_step_counter <= _time_steps);
            }
        private:
            uint32_t _step_counter = 0;
            uint32_t _time_steps;
        };

        /**
         * Stops after a specified elapsed simulation time.
         */
        class StopAtTime : public StoppingCriteria
        {

        public:
            StopAtTime(double _stop_time_)
            {
                _stop_time = _stop_time_;
            }
            virtual bool shouldContinue(double actualTime, sys::Problem* problem, field::Fields* solution)
            {
                return (actualTime<=_stop_time);
            }
        private:
            double _stop_time;
        };
    }


}

