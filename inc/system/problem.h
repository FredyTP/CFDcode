#pragma once
/*****************************************************************//**
 * \file   problem.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

 //STL
#include <memory>

//CFD
//----------MESH----------//
#include <mesh/mesh.h>

//----------MATERIAL----------//
#include <material/material.h>
#include <material/material_factory.h>

//----------BOUNDARY CONDITION----------//
#include <boundary/boundary_condition.h>
#include <boundary/constant_temperature.h>
#include <boundary/constant_gradient.h>

//----------FIELD----------//
#include <field/fields.h>
#include <field/state_vector.h>

//----------EQUATION----------//
#include <equation/equation_term.h>
#include <equation/source/source_term.h>
#include <equation/temporal/temporal_term.h>


namespace sys
{
    class Problem
    {
    public:
        Problem() {}

        //----------MESH----------//

        /**
         * Loads Mesh from file data: node_path and cell_path. With the Luismi
         * format loader
         * 
         * \param node_path path to node file
         * \param cell_path path to cell file
         */
        void loadMesh(std::string node_path ,std::string cell_path);

        /**
         * Loads the correct project mesh based on the specified number of cells.
         * 
         * \param n_cell number of cells of the mesh to be importets
         */
        void loadProjectMesh(int n_cell);

        /**
         * Creates a rectangular mesh.
         * 
         * \param w width of the mesh [m]
         * \param h height of the mesh [m]
         * \param n_w number of horizontal divisions (Cells)
         * \param n_h number of verticarl divisions (Cells)
         */
        void createRectMesh(double w, double h, int n_w, int n_h);

        //--------------------//
       
        //----------MATERIAL----------//
        /**
         * Add Material to material list with specified constant value properties
         * 
         * \param density
         * \param viscosity
         * \param conductivity
         * \param specific_heat
         */
        int addConstantMaterial(double density, double viscosity, double conductivity, double specific_heat);

        /**
         * Add Material to material list from a material factory. The
         * materialFactory must be configured first in order to correctly import the material.
         * 
         * \param materialFactory contains the material to be imported
         * \return the index of the material.
         */
        int addMaterial(material::MaterialFactory& materialFactory);

        /**
         * Assign a Material from material list at position index to the mesh selection.
         * 
         * \param index 0 based index of the material
         * \param selection selection of cells from the problem mesh
         */
        void assignMaterial(int index, mesh::MeshSelection<mesh::Cell>& selection);

        /**
         * Assign material at index to all the mesh cells
         * 
         * \param index 0 based index of the material (defaults to 0)
         */
        void assignMaterial(int index=0);
        //--------------------//

        //----------SELECTIONS----------//

        /**
         * Add a face selection to the list.
         * 
         * \param face_selection 
         * \return index in the list
         */
        int addFaceSelection(std::unique_ptr<mesh::MeshSelection<mesh::Face>>& face_selection);

        /**
         * Adds and loads a new face selection from the specified file.
         * 
         * \param path
         * \return the index of the selection in the list
         */
        int loadFaceSelection(const std::string path);

        /**
         * Loads the project face selection for boundary coundition.
         * 
         * \param index number of the selection
         * \param n_cell number of cells
         * \return the index of the selection in the list
         */
        int loadProjectFaceSelection(int index, int n_cell);
        //--------------------//

        //----------BOUNDARY CONDITION----------//

        /**
         * Adds new constant temperature boundary condition to the list.
         * A mesh selection should be assigned to the bc.
         * 
         * \param temperature
         * \return index
         */
        int addConstTempBoundary(double temperature);

        /**
         * Adds new constant temperature boundary condition to the list.
         * A mesh selection should be assigned to the bc.
         * 
         * \param flux
         * \return index
         */
        int addConstFluxBoundary(double flux);

        /**
         * Assigns a boundary selection to a face selection(MUST be made up ONLY by boundary faces).
         * 
         * \param bc boundary condition index
         * \param selection
         */
        void assignBoundaryCondition(int bc, int selection);
        //--------------------//

        //----------FIELDS----------//
        /**
         * Initialize fields based on the parameters.
         * 
         * \param velocity
         * \param scalar
         */
        void initializeFields(vector2d velocity, field::ScalarStateVector scalar);

        //--------------------//

        //----------EQUATIONS----------//

        /**
         * Init Basic equation terms.
         * 
         */
        void initEquations();

        /**
         * Changes the face interpolation method with the one specified
         * 
         * \param faceInterpolation
         */
        void configConvectiveTerm(term::FaceInterpolation* faceInterpolation);

        /**
         * Changes the gradient flux method with the one specified.
         * 
         * \param gradientFlux
         */
        void configDiffusiveTerm(term::GradientFlux* gradientFlux);
            
        //int addSourceTerm();
        //int assignSourceTerm();
        //--------------------//

        //----------GLOBAL-----------//
        /**
         * Configures all missing things that need to be done.
         * 
         */
        void buildProblem();

        //--------------------//

        //----------GETTERS-----------//
        mesh::Mesh* mesh() { return _mesh.get(); }
        
        field::Fields* fields() { return _fields.get(); }
        void for_each_boundary(std::function<void(bc::BoundaryCondition*)> on_boundary);

        void for_each_faceTerm(std::function<void(term::FaceEquationTerm*)> on_face_term);

        term::CellEquationTerm* termporalTerm() { return _temporalTerm.get(); }
        
    private:
        std::unique_ptr<mesh::Mesh> _mesh;
        std::vector<std::unique_ptr<material::Material>> _materials;
        std::vector<std::unique_ptr<bc::BoundaryCondition>> _boundaryConditions;
        std::unique_ptr<field::Fields> _fields;

        std::vector<std::unique_ptr<mesh::MeshSelection<mesh::Face>>> _faceSelections;

        std::vector<std::unique_ptr<term::FaceEquationTerm>> _faceTerms;
        term::ConvectiveTerm* _convectiveTerm;
        term::DiffusiveTerm* _diffusiveTerm;
        std::vector<std::unique_ptr<term::SourceTerm>> _sourceTerms;
        std::unique_ptr<term::TemporalTerm> _temporalTerm;


    };
}
