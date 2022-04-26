/*****************************************************************//**
 * \file   boundary_condition.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <fstream>

//CFD
#include <mesh/base/face.h>
#include <mesh/manipulation/mesh_selection.h>
#include <field/fields.h>
#include <equation/convective/convective_term.h>
#include <math/var/system_submatrix.h>
#include <equation/diffusive/diffusive_term.h>
#include <equation/equation.h>
namespace bc
{

class BoundaryCondition
{
public:
    BoundaryCondition() {}
    virtual void calculateBoundaryCondition(math::SystemSubmatrix *submatrix,const field::Fields* field, field::scalarType equation) const
    {     
        for (auto& face : _boundary)
        {
            this->getBoundaryCondition(submatrix, face->cell2(), field, equation);
        }
    };
    virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell,const field::Fields* field, field::scalarType equation) const = 0;
    void addFace(mesh::Face* _face_)
    {
        _boundary.push_back(_face_);
    }
    void setBoundary(const mesh::MeshSelection<mesh::Face>& boundary)
    {
        boundary.copyInto(_boundary);
    }
    
    void setEquations(std::array<std::unique_ptr<eq::Equation>, field::scalar_field_number>* _equations_)
    {
        _equations = _equations_;
    }

protected:

    std::vector<mesh::Face*> _boundary;
    std::vector<mesh::Cell*> _boundaryCells;
    std::array<std::unique_ptr<eq::Equation>,field::scalar_field_number>* _equations;
};

}
