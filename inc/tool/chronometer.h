/*****************************************************************//**
 * \file   chronometer.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#pragma once

#include <chrono>
#include <iostream>

namespace tool
{
    /**
     * Class for time measurement.
     */
    class Chronometer
    {
    public:
        Chronometer(){}
        void tic()
        {
            _begin = std::chrono::steady_clock::now();
        }
        double tac(bool print = false, const std::string action = "")
        {
            _end = std::chrono::steady_clock::now();
            if (print)
            {
                std::cout << "Elapsed time " << action << ": " << this->getElapsedMS() << "[ms]" << std::endl;
            }
            return this->getElapsedMS();
        }
        double getElapsedMS()
        {
            return (double)std::chrono::duration_cast<std::chrono::microseconds>(_end - _begin).count() / 1000.0;
        }
    private:
        std::chrono::steady_clock::time_point _begin;
        std::chrono::steady_clock::time_point _end;
        
    };
}
