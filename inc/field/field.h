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

#include <mesh/mesh.h>
#include <field/state_vector.h>

#include <mesh/base/face.h>
#include <mesh/base/cell.h>
#include <mesh/base/vector.h>

namespace field
{
    enum scalar
    {
        density=0,
        temperature,
        pressure,

    };
    enum vectorial
    {
        velocity=0,
        
    };
class Fields
{
public:
    Fields(const mesh::Mesh* _mesh_);
    void copy(const Fields* other);

    const vector2d& velocityField(const mesh::Face* face) const;        //Face centroids
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
    Eigen::VectorX<vector2d>& rawVelocity() { return _velocityField; }
    
private:
    
    Eigen::VectorX<vector2d> _velocityField;  //Face centroids
    Eigen::VectorX<double> _densityField;     //Cell centroids
    Eigen::VectorX<double> _temperatureField; //Cell centroids
    Eigen::VectorX<double> _pressureField;    //Cell centroids

    const mesh::Mesh* _mesh; //Pointer to the mesh represented

};

}

