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

namespace material
{

namespace prop
{


class IdealGasDensity : public DensityBase
{
public:
    IdealGasDensity(double _refDensity_) : _refDensity(_refDensity_)
    {
        _gasConstant = _refDensity * _refTemperature / _refPressure;

    }
    virtual double density(const mesh::StateVector& state) final {
        return _gasConstant * state.pressure / state.temperature;
    }
    ~IdealGasDensity() override {}
private:
    double _refDensity; //Density at (298,15 K - 101325 Pa)
    double _gasConstant;
    static const double _refTemperature;
    static const double _refPressure;

};

const double IdealGasDensity::_refTemperature = 298.15; //K
const double IdealGasDensity::_refPressure = 101325; //Pa

}

}