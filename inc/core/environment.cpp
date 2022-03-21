#include "environment.h"
#include <mesh/reader/luismi_format_mesh_reader.h>
#include <material/material_factory.h>
#include <material/properties/constant_properties.h>
#include <Eigen/Sparse>
#include <boundary/constant_temperature.h>
#include <solver/matrix_builder.h>
#include <math/central_difference_gradient.h>

core::Environment::Environment()
{
}

void core::Environment::loadMesh()
{
    using namespace mesh;
    LuisMiformatMeshReader meshReader("D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//nodes_4096.dat",
    "D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//cells_4096.dat");
    _mesh = std::make_unique<Mesh>();
    _mesh->loadMesh(&meshReader);
}

void core::Environment::computeMesh()
{
    
}

void core::Environment::buildMaterials()
{
    using namespace material;
    MaterialFactory matFactory;
    matFactory.setDensityModel<prop::ConstantDensity>(1.225);
    matFactory.setViscosityModel<prop::ConstantViscosity>(1e-5);
    matFactory.setConductivityModel<prop::ConstantConductivity>(1e4);
    matFactory.setSpecificHeatModel<prop::ConstantSpecificHeat>(1040);

    _fluidMaterial = matFactory.extractMaterial();
}

void core::Environment::createBoundary()
{
    std::unique_ptr<bc::BoundaryCondition> uniquebc = std::make_unique<bc::ConstantTemperature>(300);
    for (size_t faceid = 0; faceid < _mesh->faces()->size(); faceid++)
    {
        auto face = _mesh->faces()->at(faceid).get();
        if (face->isBoundary())
        {
            uniquebc->addFace(face);
        }
    }
    _boundaryConditions.push_back(std::move(uniquebc));
}

void core::Environment::initializeFields()
{
    using namespace field;
    _fields = std::make_unique<Field>(_mesh.get());
    for (size_t i = 0; i < _fields->scalarFields()->size(); i++)
    {
        auto escalarField = _fields->scalarFields();
        (*escalarField)[i].density = _fluidMaterial->density((*escalarField)[i]);
        (*escalarField)[i].pressure = 101325;
        (*escalarField)[i].temperature = 298.15;
    }
    for (size_t i = 0; i < _fields->velocityField()->size(); i++)
    {
        auto velField = _fields->velocityField();
        (*velField)[i].x() = 1;
        (*velField)[i].y() = 1;
       
    }
    
}

void core::Environment::solve()
{
    
    
    solver::MatrixBuilder matrix;
    math::CentralDifferenceGradient gradScheme;
    _mesh->computeUnitFlux((math::GradientScheme*)(&gradScheme));
    matrix.buildSystem(_mesh.get(), (math::GradientScheme*)(&gradScheme),_boundaryConditions.front().get(),_fields.get());
    



}
