/*****************************************************************//**
 * \file   material.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <field/state_vector.h>
#include <material/properties/properties_base.h>
namespace material
{

class MaterialFactory;
class Material
{
    friend MaterialFactory;
public:
    Material() {}
    Material(std::unique_ptr<prop::DensityBase>& density, std::unique_ptr <prop::ViscosityBase>& viscosity, std::unique_ptr <prop::ConductivityBase>& conductivity)
    {
        _density = std::move(density);
        _viscosity = std::move(viscosity);
        _conductivity = std::move(conductivity);
    }
    double density(const field::ScalarStateVector& state) const { return _density->density(state); };
    double viscosity(const field::ScalarStateVector& state) const { return _viscosity->viscosity(state); }
    double conductivity(const field::ScalarStateVector& state) const { return _conductivity->conductivity(state); };
    double specificHeat(const field::ScalarStateVector& state) const { return _specificHeat->specificHeat(state); };
private:
    std::unique_ptr<prop::DensityBase> _density;
    std::unique_ptr<prop::ViscosityBase> _viscosity;
    std::unique_ptr<prop::ConductivityBase> _conductivity;
    std::unique_ptr<prop::SpecificHeatBase> _specificHeat;
};



}