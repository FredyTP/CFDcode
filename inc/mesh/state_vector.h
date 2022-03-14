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

namespace mesh
{


struct StateVector
{
    vector2d velocity;
    double density;
    double temperature;
    double pressure;

};


}

