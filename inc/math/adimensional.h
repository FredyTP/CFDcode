#pragma once
/*****************************************************************//**
 * \file   adimensional.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

namespace math
{
    class Adimensional
    {
    public:
        static double MachNumber(double speed, double soundSpeed)
        {
            return speed / soundSpeed;
        }
        static double PecletNumber(double cp,double density, double speed, double longitude, double thermal_conductivity)
        {
            return cp*density * speed * longitude / thermal_conductivity;            
        }
        static double Lambda(double cell2face, double cell2cell)
        {
            return cell2face / cell2cell;
        }
    };

}
