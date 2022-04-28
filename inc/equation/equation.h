/*****************************************************************//**
 * \file   equation.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#pragma once

#include <field/fields.h>
#include <equation/equation_term.h>
#include <equation/diffusive/diffusive_term.h>
#include <equation/convective/convective_term.h>
namespace eq
{
    class Equation
    {
    public:
        Equation(field::scalarType _scalar_) : _scalar(_scalar_) {}
        void initTerms(term::FaceInterpolation* faceInterpolation, term::GradientFlux* gradientFlux)
        {
            if(faceInterpolation !=nullptr)
            _convectiveTerm = std::make_unique<term::ConvectiveTerm>(faceInterpolation, _scalar);

            if (gradientFlux != nullptr)
            _diffusiveTerm = std::make_unique<term::DiffusiveTerm>(gradientFlux, _scalar);
        }
        term::ConvectiveTerm* convective() { return _convectiveTerm.get(); }
        term::DiffusiveTerm* diffusive() { return _diffusiveTerm.get(); }
    private:
        std::unique_ptr<term::ConvectiveTerm> _convectiveTerm;
        std::unique_ptr <term::DiffusiveTerm> _diffusiveTerm;
        field::scalarType _scalar;
    };
}
