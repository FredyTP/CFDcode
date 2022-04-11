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
        inline double MachNumber(double speed, double soundSpeed)
        {
            return speed / soundSpeed;
        }
        inline double PecletNumber(double density, double speed, double longitude, double thermal_conductivity)
        {
            return density * speed * longitude / thermal_conductivity;            
        }
    };

}
