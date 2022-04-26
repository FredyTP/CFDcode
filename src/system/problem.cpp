/*****************************************************************//**
 * \file   problem.cpp
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#include <system/problem.h>

#include <mesh/reader/luismi_format_mesh_reader.h>


namespace sys
{
    void Problem::loadMesh(std::string node_path, std::string cell_path)
    {
        using namespace mesh;
        LuisMiformatMeshReader meshReader(node_path, cell_path);
        _mesh = std::make_unique<Mesh>();
        _mesh->loadMesh(&meshReader);
    }
    void Problem::loadProjectMesh(int n_cell)
    {
        using namespace mesh;       
        std::string base_mesh_path = "mesh//";
        std::string format = ".dat";
        std::string node_path = base_mesh_path + "nodes_" + std::to_string(n_cell) + format;
        std::string cell_path = base_mesh_path + "cells_" + std::to_string(n_cell) + format;
        this->loadMesh(node_path, cell_path);

    }
    int Problem::addConstantMaterial(double density, double viscosity, double conductivity, double specific_heat)
    {
        material::MaterialFactory matFact;
        matFact.constantMaterial(density, viscosity, conductivity, specific_heat);
        return this->addMaterial(matFact);
    }
    int Problem::addMaterial(material::MaterialFactory& materialFactory)
    {
        _materials.push_back(materialFactory.extractMaterial());
        return _materials.size() - 1;
    }
    void Problem::assignMaterial(int index, mesh::MeshSelection<mesh::Cell>& selection)
    {
        auto material = _materials.at(index).get();
        
        selection.for_each([material](mesh::Cell* cell) 
            {
                cell->setMaterial(material);
            });
    }
    void Problem::assignMaterial(int index)
    {
        mesh::MeshSelection<mesh::Cell> all;
        all.selectAll(_mesh.get());

        assignMaterial(index, all);
    }
    int Problem::loadFaceSelection(const std::string path)
    {
        mesh::reader::FaceReader faceReader(path);
        std::unique_ptr<mesh::MeshSelection<mesh::Face>> selection = std::make_unique<mesh::MeshSelection<mesh::Face>>();

        selection->selectWithReader(_mesh.get(), &faceReader);
        _faceSelections.push_back(std::move(selection));
        return _faceSelections.size() - 1;
    }
    int Problem::loadProjectFaceSelection(int index, int n_cell)
    {
        std::string path = "mesh//bc";
        std::string format = ".dat";
        std::string sel_path = path + "_" + std::to_string(index) + "_" + std::to_string(n_cell) + format;
        return loadFaceSelection(sel_path);
    }
    int Problem::addConstTempBoundary(double temperature)
    {
        std::unique_ptr<bc::BoundaryCondition> bc = std::make_unique<bc::ConstantTemperature>(temperature);

        _boundaryConditions.push_back(std::move(bc));

        return _boundaryConditions.size() - 1;
    }
    int Problem::addConstFluxBoundary(double flux)
    {
        std::unique_ptr<bc::BoundaryCondition> bc = std::make_unique<bc::ConstantFlux>(flux);

        _boundaryConditions.push_back(std::move(bc));

        return _boundaryConditions.size() - 1;
    }
    void Problem::assignBoundaryCondition(int bc, int selection)
    {
        _boundaryConditions[bc]->setBoundary(*_faceSelections[selection].get());
    }
    void Problem::initializeFields(vector2d velocity, field::ScalarStateVector scalar)
    {
        using namespace field;

        _fields = std::make_unique<Fields>(_mesh.get());
        _fields->init();
        auto& densityField = _fields->scalarField(field::density);
        auto& tempField = _fields->scalarField(field::temperature);
        auto& pressField = _fields->scalarField(field::pressure);
        auto& velFieldx = _fields->scalarField(field::velocity_x);
        auto& velFieldy = _fields->scalarField(field::velocity_y);

        for (size_t i = 0; i < densityField.size(); i++)
        {

            pressField[i] = 101325;
            tempField[i] = 298.15;
            //THIS IS A DERIVED FIELD SO... CHANGE THAT
            densityField[i] = _mesh->cells()->at(i)->material()->density(_mesh->cells()->at(i).get(), _fields.get());
            velFieldx[i] = velocity.x();
            velFieldy[i] = velocity.y();
        }

            
    
    }

    void Problem::initEquation(field::scalarType scalar, term::FaceInterpolation* faceInterpolation, term::GradientFlux* gradientFlux)
    {
        _equations[scalar] = std::make_unique<eq::Equation>(scalar);
        _equations[scalar]->initTerms(faceInterpolation, gradientFlux);
    }

    void Problem::buildProblem()
    {
        //FIND MORE ELEGANT WAY
        for (auto& boundary : _boundaryConditions)
        {
            boundary->setEquations(&_equations);
        }
    }
    void Problem::for_each_boundary(std::function<void(bc::BoundaryCondition*)> on_boundary)
    {
        for (auto& boundary : _boundaryConditions)
        {
            on_boundary(boundary.get());
        }
    }
    void Problem::for_each_equation(std::function<void(eq::Equation*)> on_equation)
    {
        for(auto& equation : _equations)
        {
            on_equation(equation.get());
        }
    }
}
