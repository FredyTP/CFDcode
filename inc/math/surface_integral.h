#pragma once
/*****************************************************************//**
 * \file   SurfaceIntegral.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
namespace math
{

class SurfaceIntegral
{
public:
    /**
     * \brief Calculates the numeric integral O(Δx^2) with midpoint value.
     * 
     * \param area Area of the face on surface integral is done
     * \param f_mid Value of the integrand in the face centroid
     * \return Value of the numeric midpoint integral
     */
    template<class type> static type midpoint(double area, type f_mid) {
        return area * f_mid;
    }

    /**
     * \brief Calculates the numeric integral O(Δx^2) with vertex values.
     * 
     * \param area Area of the face on surface integral is done
     * \param f_vert1 Value of the integrand in the vertex 1
     * \param f_vert2 Value of the integrand in the vertex 2
     * \return Value of the numeric trapezoidal integral
     */
    template<class type> static type trapezoidal(double area, type f_vert1,type f_vert2) {
        return area * (f_vert1 + f_vert2) / 2.0;
    }

    /**
     * \brief Calculates the numeric integral O(Δx^4) with midpoint and vertex values.
     * 
     * \param area Area of the face on surface integral is done
     * \param f_mid
     * \param f_vert1 Value of the integrand in the vertex 1
     * \param f_vert2 Value of the integrand in the vertex 2
     * \return Value of the numeric simpsons integral
     */
    template<class type> static type simpsons(double area, type f_mid, type f_vert1, type f_vert2) {
        return area * (f_vert1 + 4.0 * f_mid + f_vert2) / 6.0;
    }


};


}
