/*****************************************************************//**
 * \file   properties_base.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <mesh/state_vector.h>
namespace material
{
namespace prop
{

class DensityBase
{
public:
    virtual double density(const mesh::StateVector& state) = 0;
    virtual ~DensityBase() {};

};
class ViscosityBase
{
public:
    virtual double viscosity(const mesh::StateVector& state) = 0;
    virtual ~ViscosityBase()  {};
};
class ConductivityBase
{
public:
    virtual double conductivity(const mesh::StateVector& state) = 0;
    virtual ~ConductivityBase() {};
};


}
}