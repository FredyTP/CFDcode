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
#include <field/field.h>
#include <equation/convective/convective_term.h>
#include <math/var/system_submatrix.h>
#include <equation/diffusive/diffusive_term.h>
namespace bc
{

class BoundaryCondition
{
public:
    BoundaryCondition() {}
    virtual void calculateBoundaryCondition(math::SystemSubmatrix *submatrix,const field::Fields* field) const
    {     
        for (auto& face : _boundary)
        {
            this->getBoundaryCondition(submatrix, face->cell2(), field);
        }
    };
    virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell,const field::Fields* field) const = 0;
    void addFace(mesh::Face* _face_)
    {
        _boundary.push_back(_face_);
    }
    void setBoundary(const mesh::MeshSelection<mesh::Face>& boundary)
    {
        boundary.copyInto(_boundary);
    }
    
    void setConvetiveTerm(term::ConvectiveTerm* _convectiveTerm_)
    {
        _convectiveTerm = _convectiveTerm_;
    }
    void setDiffusiveTerm(term::DiffusiveTerm* _diffusiveTerm_)
    {
        _diffusiveTerm = _diffusiveTerm_;
    }
protected:

    std::vector<mesh::Face*> _boundary;
    std::vector<mesh::Cell*> _boundaryCells;

    term ::ConvectiveTerm* _convectiveTerm = nullptr;
    term::DiffusiveTerm* _diffusiveTerm = nullptr;
};

}
