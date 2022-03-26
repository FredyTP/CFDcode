/*****************************************************************//**
 * \file   StateVector.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <base/global_typedef.h>

namespace field
{


struct ScalarStateVector
{
    ScalarStateVector(double _density, double _temperature, double _pressure) 
        : density(_density), temperature(_temperature), pressure(_pressure) 
    {

    }
    double density;
    double temperature;
    double pressure;

};


}

