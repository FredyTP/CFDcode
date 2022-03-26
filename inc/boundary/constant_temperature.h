/*****************************************************************//**
 * \file   constant_temperature.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <boundary/boundary_condition.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <field/field.h>
#include <math/var/system_submatrix.h>

namespace bc
{
    class ConstantTemperature : public BoundaryCondition
    {
    public:
        ConstantTemperature(double _temperature_) :_temperature(_temperature_) {
            _convectiveTerm = nullptr;
            _diffusiveTerm = nullptr;
        };
        
        virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const  mesh::Face* face, const field::Fields* field) const
        {
            
            if (_convectiveTerm != nullptr)
            {
                _convectiveTerm->calculateFaceConvectionBoundary(submatrix,cell, face, _temperature, field);;
            }
            if (_diffusiveTerm != nullptr)
            {
                _diffusiveTerm->calculateFaceDiffusionBoundary(submatrix,cell, face, _temperature, field);
                
            }
            
        }

    private:
        double _temperature;
    };
}

//OLD ONE
/*virtual std::pair<std::vector<Eigen::Triplet<double>>, double> getBoundaryCondition(const mesh::Cell* cell, const  mesh::Face* face, field::Field* field) const
{
    std::vector<Eigen::Triplet<double>> coefs;
    //THIS IS NOT GOOD --- CORRECT IN THE FUTURE -- Only works for constant values
    //field::StateVector state;
    double k = 1000;//cell->material()->conductivity(state);
    double flux = k * face->getUnitFlux(cell);
    double rho = 1.225;
    double cv = 1048;
    coefs.push_back(Eigen::Triplet<double>(cell->index(), cell->index(), flux));
    double difusiveSource = _temperature * flux;
    double convectiveSource = _temperature * cv * rho * face->area() * field->velocityField()->at(face->index()).dot(face->getNormal(cell));
    double column = (difusiveSource - convectiveSource);
    return std::pair<std::vector<Eigen::Triplet<double>>, double>(coefs, column);
}*/