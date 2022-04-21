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
        auto& densityField = _fields->rawDensity();
        auto& tempField = _fields->rawTemperature();
        auto& pressField = _fields->rawPressure();
        auto& velField = _fields->rawVelocity();

        for (size_t i = 0; i < densityField.size(); i++)
        {

            pressField[i] = 101325;
            tempField[i] = 298.15;
            //THIS IS A DERIVED FIELD SO... CHANGE THAT
            densityField[i] = _mesh->cells()->at(i)->material()->density(_fields->scalarField(_mesh->cells()->at(i).get()));
        }
        for (size_t i = 0; i < velField.size(); i++)
        {
            velField[i] = velocity;
        }
    }
    void Problem::initEquations()
    {
        using namespace term;
        auto conv = std::make_unique<ConvectiveTerm>(nullptr);
        _convectiveTerm = conv.get();

        auto diff = std::make_unique<DiffusiveTerm>(nullptr);
        _diffusiveTerm = diff.get();

        _faceTerms.push_back(std::move(conv));
        _faceTerms.push_back(std::move(diff));

        _temporalTerm = std::make_unique<term::TemporalTerm>();
    }
    void Problem::configConvectiveTerm(term::FaceInterpolation* faceInterpolation)
    {
        _convectiveTerm->setFaceInterpolation(faceInterpolation);
    }
    void Problem::configDiffusiveTerm(term::GradientFlux* gradientFlux)
    {
        _diffusiveTerm->setGradientFlux(gradientFlux);
    }
    void Problem::buildProblem()
    {
        //FIND MORE ELEGANT WAY
        for (auto& boundary : _boundaryConditions)
        {
            boundary->setConvetiveTerm(_convectiveTerm);
            boundary->setDiffusiveTerm(_diffusiveTerm);
        }
    }
    void Problem::for_each_boundary(std::function<void(bc::BoundaryCondition*)> on_boundary)
    {
        for (auto& boundary : _boundaryConditions)
        {
            on_boundary(boundary.get());
        }
    }
    void Problem::for_each_faceTerm(std::function<void(term::FaceEquationTerm*)> on_face_term)
    {
        for (auto& faceterm : _faceTerms)
        {
            on_face_term(faceterm.get());
        }
    }
}
