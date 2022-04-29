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
    class Chronometer
    {
    public:
        Chronometer(){}
        void tic()
        {
            _begin = std::chrono::steady_clock::now();
        }
        void tac(bool print = false, const std::string action = "")
        {
            _end = std::chrono::steady_clock::now();
            if (print)
            {
                std::cout << "Elapsed time " << action << ": " << this->getElapsedMS() << "[ms]" << std::endl;
            }
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
