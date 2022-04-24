/*****************************************************************//**
 * \file   ideal_gas.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <material/properties/properties_base.h>
#include <field/fields.h>
namespace material
{
namespace prop
{


class ConstantDensity : public DensityBase
{
public:
    ConstantDensity(double _refDensity_) : _refDensity(_refDensity_) {}

    virtual double density(const mesh::Cell* cell, const field::Fields* fields) final {
        return _refDensity;
    }
    ~ConstantDensity() override {}
private:
    double _refDensity;

};

class ConstantViscosity : public ViscosityBase
{
public:
    ConstantViscosity(double _refViscosity_) : _refViscosity(_refViscosity_) {}

    virtual double viscosity(const mesh::Cell* cell, const field::Fields* fields) final {
        return _refViscosity;
    }
    ~ConstantViscosity() override {}
private:
    double _refViscosity;

};

class ConstantConductivity : public ConductivityBase
{
public:
    ConstantConductivity(double _refConductivity_) : _refConductivity(_refConductivity_) {}

    virtual double conductivity(const mesh::Cell* cell, const field::Fields* fields) final {
        return _refConductivity;
    }
    ~ConstantConductivity() override {}
private:
    double _refConductivity;

};

class ConstantSpecificHeat : public SpecificHeatBase
{
public:
    ConstantSpecificHeat(double _refSpecificHeat_) : _refSpecificHeat(_refSpecificHeat_) {}

    virtual double specificHeat(const mesh::Cell* cell, const field::Fields* fields) final {
        return _refSpecificHeat;
    }
    ~ConstantSpecificHeat() override {}
private:
    double _refSpecificHeat;

};

}
}
