/*****************************************************************//**
 * \file   interpolation.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <base/global_typedef.h>
namespace math
{

    /**
     * Class containing different interpolation schemes to interpolate
     * field values in the cell face
     */
class FaceInterpolation
{
public:
    /**
     * \brief Computes the Upwind Differencing Scheme(UDS) interpolation
     * on the cell's face. 
     * 
     * Properties:
     * - First order
     * - Unconditionallly stable(no oscillations)
     * - Numerically diffusive
     * 
     * \param normal: Normal vector to the face (pointing outside the cell)
     * \param velocity: Velocity vector
     * \param phi_cell: Value at the cell center
     * \param phi_neig: Value at the cell neighbour connected by the face
     * \return Return the interpolated value on the cell
     */
    template<class type> static type uds(vector2d normal,vector2d velocity,
                                         type phi_cell, type phi_neig) {
        return (velocity.dot(normal) > 0 ? phi_cell : phi_neig);
    }

    /**
     * \brief Computes the Central Differencing Scheme (CDS) interpolation
     * on the cell's face. 
     * 
     * \param centroid_distance distance from cell centroid to neightbour cell centroid
     * \param face_distance distance form cell centroid to face centroid
     * \param phi_cell value of phi of the cell
     * \param phi_neig value of phi of neightbour cell
     * \return 
     */
    template<class type> static type cds(double centroid_distance, double face_distance,
                                     type phi_cell, type phi_neig) {
        double lambda = face_distance / centroid_distance;
        return (phi_neig * lambda + phi_cell * (1 - lambda));
    }

    //UNIMPLEMENTED INTERPOLATION METHODS
    //template<class type> static type quick()=0;
    //template<class type> static type power_law() = 0;
};


}

