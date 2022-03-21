/*****************************************************************//**
 * \file   environment.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
//STL

//CFD
#include <mesh/mesh.h>
#include <material/material.h>
#include <field/field.h>
#include <math/gradient.h>
#include <boundary/boundary_condition.h>
namespace core
{

class Environment
{
public:
    Environment();

    void loadMesh();
    void computeMesh();
    void buildMaterials();
    void createBoundary();
    void initializeFields();
    void solve();
private:
    std::unique_ptr<mesh::Mesh> _mesh;
    std::unique_ptr<material::Material> _fluidMaterial;
    std::unique_ptr<field::Field> _fields;
    std::unique_ptr<math::GradientScheme> _gradientScheme;
    std::list<std::unique_ptr<bc::BoundaryCondition>> _boundaryConditions;
    //FLUID DOMAIN /-> 
    //BOUNDARY CONDITIONS /-> Boundary condition type and material (?)
    //Solver /->Interpolation and integral aproximations, ecuations, etc..
    //
};
}
