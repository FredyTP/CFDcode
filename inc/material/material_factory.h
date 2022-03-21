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
#include <memory>


namespace material
{
class MaterialFactory
{
public:
    MaterialFactory()
    {
        this->reset();
    }
    void reset()
    {
        _material = std::make_unique<Material>();
    }
    template <class _DensityModel, class... _Types> void setDensityModel(_Types&&... _Args)
    {
        _material->_density = std::make_unique<_DensityModel>(_Args...);
    }
    template <class _ViscosityModel, class... _Types> void setViscosityModel(_Types&&... _Args)
    {
        _material->_viscosity = std::make_unique<_ViscosityModel>(_Args...);
    }
    template <class _ConductivityModel, class... _Types> void setConductivityModel(_Types&&... _Args)
    {
        _material->_conductivity = std::make_unique<_ConductivityModel>(_Args...);
    }
    template <class _SpecificHeatModel, class... _Types> void setSpecificHeatModel(_Types&&... _Args)
    {
        _material->_specificHeat = std::make_unique<_SpecificHeatModel>(_Args...);
    }
    std::unique_ptr<Material> extractMaterial()
    {
        auto temp = std::move(_material);
        this->reset();
        return std::move(temp);
    };

private:
    std::unique_ptr<Material> _material;
};




}
