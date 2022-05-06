/*****************************************************************//**
 * \file   properties_base.h
 * \brief  Define the properties virtual interfaces to implement
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <field/fields.h>

namespace material
{
namespace prop
{

class DensityBase
{
public:
    virtual double density(const mesh::Cell* cell, const field::Fields* fields) = 0;
    virtual ~DensityBase() {};

};
class ViscosityBase
{
public:
    virtual double viscosity(const mesh::Cell* cell, const field::Fields* fields) = 0;
    virtual ~ViscosityBase()  {};
};
class ConductivityBase
{
public:
    virtual double conductivity(const mesh::Cell* cell, const field::Fields* fields) = 0;
    virtual ~ConductivityBase() {};
};
class SpecificHeatBase
{
public:
    virtual double specificHeat(const mesh::Cell* cell, const field::Fields* fields) = 0;
    virtual ~SpecificHeatBase() {};
};

}
}