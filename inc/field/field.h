/*****************************************************************//**
 * \file   field.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <vector>

//CFD
#include <mesh/mesh.h>
#include <field/state_vector.h>
namespace field
{

class Field
{
public:
    Field(const mesh::Mesh* _mesh_);
    std::vector<vector2d>* velocityField() { return &_velocityField; }; //Face centroids
    std::vector<StateVector>* scalarFields() { return &_scalarFields; }; //Cell centroids
private:
    std::vector<vector2d> _velocityField; //Face centroids
    std::vector<StateVector> _scalarFields; //Cell centroids
    const mesh::Mesh* _mesh; //Pointer to the mesh represented

};

}

