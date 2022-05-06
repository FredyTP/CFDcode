/*****************************************************************//**
 * \file   material.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <material/properties/properties_base.h>
#include <field/fields.h>
namespace material
{

class MaterialFactory;
class mesh::Cell;

/**
 * Represents a Fluid or Solid Material.
 */
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
    double density(const mesh::Cell* cell, const field::Fields* fields) const { return _density->density(cell,fields); };
    double viscosity(const mesh::Cell* cell, const field::Fields* fields) const { return _viscosity->viscosity(cell,fields); }
    double conductivity(const mesh::Cell* cell, const field::Fields* fields) const { return _conductivity->conductivity(cell,fields); };
    double specificHeat(const mesh::Cell* cell, const field::Fields* fields) const { return _specificHeat->specificHeat(cell,fields); };
private:
    std::unique_ptr<prop::DensityBase> _density;
    std::unique_ptr<prop::ViscosityBase> _viscosity;
    std::unique_ptr<prop::ConductivityBase> _conductivity;
    std::unique_ptr<prop::SpecificHeatBase> _specificHeat;
};



}