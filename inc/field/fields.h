/*****************************************************************//**
 * \file   field.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once


//CFD
#include <base/global_typedef.h>

namespace mesh
{
    class Mesh;
    class Face;
    class Cell;
}

namespace field
{
    enum scalarType
    {
        //BASIC FIELDS
        temperature = 0,
        pressure,

        //DERIVED FIELD (from state equation in pressure-based algorithmss)
        density,

        //TURBULENCE FIELDS
        k_turbulence,
        w_turbulence,
        epsilon_turbulence,

        //GEOMETRY FIELDS (Only depend on geometric data)
        

        scalar_field_number, //ACOUNTS FOR THE TOTAL NUMBER OF SCALAR FIELDS

    };
    enum vectorialType
    {
        //GEOMETRY FIELDS (Only depend on geometric data)
        cell_least_squares_gradient = 0, 

        vectorial_field_number, //ACOUNTS FOR THE TOTAL NUMBER OF VECTORIAL FIELDS
    };

class Fields
{
public:
    /**
     * Contructor: assigns the mesh and enables temperature,
     * pressure and density fields.
     * 
     * \param _mesh_ the mesh over the field is created
     */
    Fields(const mesh::Mesh* _mesh_);

    /**
     * Resize enabled fields to the mesh size and set values to 0.
     * 
     */
    void init();


    /**
     * Copy other fields values and enabled configuration.
     * 
     * \param other field to copy data from
     */
    void copy(const field::Fields* other);

    /**
     * Enable the selected scalar field to be used.
     * 
     * \param field scalar field to be enabled
     * \param enable if true enables the field, if false disables
     */
    void enableScalarField(scalarType field, bool enable = true);

    /**
     * Enable the selected vector field to be used.
     * 
     * \param field vector field to be enabled
     * \param enable
     */
    void enableVectorField(vectorialType field, bool enable = true);

    /**
     * Get a reference to the scalar field selected (should be enabled).
     * 
     * \param field field selected
     * \return reference to the scalar field selected
     */
    Eigen::VectorX<double>& scalarField(scalarType field);
    /**
     * Get a CONST reference to the scalar field selected (should be enabled).
     *
     * \param field field selected
     * \return 
     */
    const Eigen::VectorX<double>& scalarField(scalarType field) const;

    /**
     * Get a reference to scalar field value of a cell.
     * 
     * \param field field selected
     * \param cell
     * \return 
     */
    double& scalarField(scalarType field, const mesh::Cell* cell);

     /**
     * Get the scalar field value of a cell.
     *
     * \param field field selected
     * \param cell
     * \return
     */
    double scalarField(scalarType field, const mesh::Cell* cell) const;

    /**
     * Get a reference to the specified vectorial field.
     * 
     * \param field field selected
     * \return 
     */
    Eigen::VectorX<vector2d>& vectorialField(scalarType field);

    /**
     * Get a CONST reference to the specified vectorial field.
     *
     * \param field field selected
     * \return
     */
    const Eigen::VectorX<vector2d>& vectorialField(scalarType field) const;


    /**
     * Get a reference to vector field value of a cell..
     * 
     * \param field field selected
     * \param cell
     * \return 
     */
    vector2d& vectorialField(scalarType field, const mesh::Cell* cell);

    /**
     * Get a CONST reference to vector field value of a cell..
     *
     * \param field field selected
     * \param cell
     * \return
     */
    const vector2d& vectorialField(scalarType field, const mesh::Cell* cell) const;

    /**
     * Get a reference to the velocity field.
     * 
     * \return 
     */
    Eigen::VectorX<vector2d>& velocityField();

    /**
     * Get a COSNT reference to the velocity field.
     *
     * \return reference to velocity field
     */
    const Eigen::VectorX<vector2d>& velocityField() const;

    /**
     * Get a reference to the veloicity field of a face.
     * 
     * \param face selected face
     * \return reference to velocity at face
     */
    vector2d& velocityField(const mesh::Face* face);

    /**
     * Get a CONST reference to the veloicity field of a face.
     *
     * \param face selected face
     * \return const reference to velocity at face
     */
    const vector2d& velocityField(const mesh::Face* face) const;

    const mesh::Mesh* mesh() const { return _mesh; }
private:

    //VELOCITY FIELD (special)
    Eigen::VectorX<vector2d> _velocity;         //Face centroids


    //----VECTORIAL FIELDS----//
    const mesh::Mesh* _mesh; //Pointer to the mesh represented

    std::array<bool, scalar_field_number> _activeScalarFields;
    std::array<bool, vectorial_field_number> _activeVectorFields;

    //----SCALAR FIELDS POINTER----//
    std::array<Eigen::VectorX<double>, scalar_field_number> _scalarFields;

    //----VECTORIAL FIELDS POINTER----//
    std::array<Eigen::VectorX<vector2d>, vectorial_field_number> _vectorialFields;
};

}

/*const vector2d& velocityField(const mesh::Face* face) const;        //Face centroids
const ScalarStateVector& scalarField(const mesh::Cell* cell) const; //Cell centroids

const vector2d& velocityField(size_t index) const;        //Face centroids
const ScalarStateVector& scalarField(size_t index) const; //Cell centroids

const Eigen::VectorX<double>& density() const { return _densityField; }
const Eigen::VectorX<double>& temperature() const { return _temperatureField; }
const Eigen::VectorX<double>& pressure() const { return _pressureField; }
const Eigen::VectorX<vector2d>& velocity() const { return _velocityField; }


Eigen::VectorX<double>& rawDensity() { return _densityField; }
Eigen::VectorX<double>& rawTemperature() { return _temperatureField; }
Eigen::VectorX<double>& rawPressure() { return _pressureField; }
Eigen::VectorX<vector2d>& rawVelocity() { return _velocityField; }*/
