/*****************************************************************//**
 * \file   material_factory.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <material/material.h>
#include <material/properties/properties_base.h>
#include <material/properties/constant_properties.h>
#include <memory>


namespace material
{
/**
* Class that builds up materials.
*/
class MaterialFactory
{
public:
    MaterialFactory()
    {
        this->reset();
    }
    /**
     * Delete saved material and init new one.
     * 
     */
    void reset()
    {
        _material = std::make_unique<Material>();
    }
    /**
     * Set the density class.
     * 
     * \param ..._Args
     */
    template <class _DensityModel, class... _Types> void setDensityModel(_Types&&... _Args)
    {
        _material->_density = std::make_unique<_DensityModel>(_Args...);
    }

    /**
    * Set the Viscosity class.
    *
    * \param ..._Args
    */
    template <class _ViscosityModel, class... _Types> void setViscosityModel(_Types&&... _Args)
    {
        _material->_viscosity = std::make_unique<_ViscosityModel>(_Args...);
    }

    /**
    * Set the Conductivity class.
    *
    * \param ..._Args
    */
    template <class _ConductivityModel, class... _Types> void setConductivityModel(_Types&&... _Args)
    {
        _material->_conductivity = std::make_unique<_ConductivityModel>(_Args...);
    }
    /**
     * Set the SpecificHeat class.
     *
     * \param ..._Args
     */
    template <class _SpecificHeatModel, class... _Types> void setSpecificHeatModel(_Types&&... _Args)
    {
        _material->_specificHeat = std::make_unique<_SpecificHeatModel>(_Args...);
    }

    /**
     * Extract created material, returning the smart pointer containing it 
     * and changing the ownership to the returned value.
     * 
     * \return Ownership of created material
     */
    std::unique_ptr<Material> extractMaterial()
    {
        auto temp = std::move(_material);
        this->reset();
        return std::move(temp);
    };

    /**
     * Builds a constant properties material with specified values
     * 
     * \param density
     * \param viscosity
     * \param conductivity
     * \param specific_heat
     */
    void constantMaterial(double density,double viscosity,double conductivity,double specific_heat)
    {
        this->reset();
        this->setDensityModel<prop::ConstantDensity>(density);
        this->setViscosityModel<prop::ConstantViscosity>(viscosity);
        this->setConductivityModel<prop::ConstantConductivity>(conductivity);
        this->setSpecificHeatModel<prop::ConstantSpecificHeat>(specific_heat);
    };
    

private:
    std::unique_ptr<Material> _material;
};




}
