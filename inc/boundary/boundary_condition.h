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
namespace bc
{
/**
 * \brief Abstract class representing boundary condition.
 */
class BoundaryCondition
{
public:
    BoundaryCondition() {}
    /**
     * \brief Calculate the boundary condition terms for every boundary face.
     * 
     * \param submatrix 
     * \param field
     * \param actualTime current simulation Time [s]
     */
    virtual void calculateBoundaryCondition(math::SystemSubmatrix *submatrix,const field::Fields* field, double actualTime) const
    {     
        for (auto& face : _boundary)
        {
            //Boundary cells are the face2 of the boundary faces
            this->getBoundaryCondition(submatrix, face->cell2(), field, actualTime);
        }
    };
    /**
     * \brief Function to be overriden by derived classes 
     * that contains how to calculate the boundary condition for one boundary cell.
     * 
     * \param submatrix
     * \param cell boundary cell
     * \param field
     * \param actualTime current simulation Time [s]
     */
    virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell,const field::Fields* field, double actualTime) const = 0;

    /**
     * Add face to the boundary condition.
     * 
     * \param _face_
     */
    void addFace(mesh::Face* _face_)
    {
        _boundary.push_back(_face_);
    }

    /**
     * Set the face selection faces to the boundary faces.
     * 
     * \param boundary face selection of boundary faces
     */
    void setBoundary(const mesh::MeshSelection<mesh::Face>& boundary)
    {
        boundary.copyInto(_boundary);
    }
    
    /**
     * Set the convective term.
     * 
     * \param _convectiveTerm_
     */
    void setConvetiveTerm(term::ConvectiveTerm* _convectiveTerm_)
    {
        _convectiveTerm = _convectiveTerm_;
    }

    /**
     * Set the diffusive term.
     * 
     * \param _diffusiveTerm_
     */
    void setDiffusiveTerm(term::DiffusiveTerm* _diffusiveTerm_)
    {
        _diffusiveTerm = _diffusiveTerm_;
    }
protected:

    std::vector<mesh::Face*> _boundary;

    term ::ConvectiveTerm* _convectiveTerm = nullptr;
    term::DiffusiveTerm* _diffusiveTerm = nullptr;
};

}
