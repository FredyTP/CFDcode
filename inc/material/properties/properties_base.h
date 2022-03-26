/*****************************************************************//**
 * \file   properties_base.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <field/state_vector.h>
namespace material
{
namespace prop
{

class DensityBase
{
public:
    virtual double density(const field::ScalarStateVector& state) = 0;
    virtual ~DensityBase() {};

};
class ViscosityBase
{
public:
    virtual double viscosity(const field::ScalarStateVector& state) = 0;
    virtual ~ViscosityBase()  {};
};
class ConductivityBase
{
public:
    virtual double conductivity(const field::ScalarStateVector& state) = 0;
    virtual ~ConductivityBase() {};
};
class SpecificHeatBase
{
public:
    virtual double specificHeat(const field::ScalarStateVector& state) = 0;
    virtual ~SpecificHeatBase() {};
};

}
}