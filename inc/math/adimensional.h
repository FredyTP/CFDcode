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

        /**
         * Mach Number.
         * 
         * \param speed
         * \param soundSpeed
         * \return 
         */
        static double MachNumber(double speed, double soundSpeed)
        {
            return speed / soundSpeed;
        }

        /**
         * Thermal Peclet Number.
         * 
         * \param cp
         * \param density
         * \param speed
         * \param longitude
         * \param thermal_conductivity
         * \return 
         */
        static double PecletNumber(double cp,double density, double speed, double longitude, double thermal_conductivity)
        {
            return cp*density * speed * longitude / thermal_conductivity;            
        }

        /**
         * Lambda value, relation of cell distance to face related to cell distance to the other cell.
         * 
         * \param cell2face
         * \param cell2cell
         * \return 
         */
        static double Lambda(double cell2face, double cell2cell)
        {
            return cell2face / cell2cell;
        }
    };

}
