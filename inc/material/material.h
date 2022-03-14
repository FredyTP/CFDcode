/*****************************************************************//**
 * \file   material.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <mesh/state_vector.h>
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
    double density(const mesh::StateVector& state) { return _density->density(state); };
    double viscosity(const mesh::StateVector& state) { return _viscosity->viscosity(state); }
    double conductivity(const mesh::StateVector& state) { return _conductivity->conductivity(state); };
private:
    std::unique_ptr<prop::DensityBase> _density;
    std::unique_ptr<prop::ViscosityBase> _viscosity;
    std::unique_ptr<prop::ConductivityBase> _conductivity;
};



}