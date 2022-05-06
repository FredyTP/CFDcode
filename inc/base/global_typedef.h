/*****************************************************************//**
 * \file   global_typedef.h
 * \brief  Global type definition and constants
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <Eigen/Eigen>
#include <memory>

//TYPEDEF DEFINITIONS
/**
 * Typedef for bidimensional vectors.
 */
typedef Eigen::Vector2d vector2d;

/**
 * Typedef for bidimentional vectors.
 */
typedef Eigen::Vector3d vector3d;

//CONSTANTS
namespace env
{
    /**
     * For debug purposes.
     */
    constexpr bool is_debug__ = false;

    /**
     * Prints extra information if its debugging.
     */
    constexpr bool show_debug_print__ = false;
}




