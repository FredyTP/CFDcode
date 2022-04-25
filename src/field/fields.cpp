/*****************************************************************//**
 * \file   fields.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <field/fields.h>
#include <mesh/mesh.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

namespace field
{
    Fields::Fields(const mesh::Mesh* _mesh_)
    {
        _mesh = _mesh_;
        _activeScalarFields.fill(false);
        _activeVectorFields.fill(false);

        enableScalarField(temperature);
        enableScalarField(pressure);
        enableScalarField(density);

    }

    void Fields::init()
    {
        size_t n_cell = _mesh->cells()->size();
        size_t n_face = _mesh->faces()->size();

        _velocity.resize(n_face);
        _velocityX.resize(n_face);
        _velocityY.resize(n_face);
        _velocity.setConstant(vector2d(0, 0));

        for (int i = 0; i < _activeScalarFields.size(); i++)
        {
            if (_activeScalarFields[i])
            {
                _scalarFields[i].resize(n_cell);
                _scalarFields[i].setZero();
            }
        }

        for (int i = 0; i < _activeVectorFields.size(); i++)
        {
            if (_activeVectorFields[i])
            {
                _vectorialFields[i].resize(n_cell);
                _vectorialFields[i].setZero();
            }
        }
    }
    void Fields::copy(const field::Fields* other)
    {
        _mesh = other->_mesh;
        size_t n_cell = _mesh->cells()->size();
        size_t n_face = _mesh->faces()->size();

        for (int i = 0; i < _activeScalarFields.size(); i++)
        {
            _activeScalarFields[i] = other->_activeScalarFields[i];
            _scalarFields[i] = other->_scalarFields[i];
        }
        for (int i = 0; i < _activeVectorFields.size(); i++)
        {
            _activeVectorFields[i] = other->_activeVectorFields[i];
            _vectorialFields[i] = other->_vectorialFields[i];       
        }

    }

    void Fields::enableScalarField(scalarType field, bool enable)
    {
        _activeScalarFields[field] = enable;
    }
    void Fields::enableVectorField(vectorialType field, bool enable)
    {
        _activeVectorFields[field] = enable;
    }

    Eigen::VectorX<double>& Fields::scalarField(scalarType field)
    {
        return _scalarFields[field];
    }

    const Eigen::VectorX<double>& Fields::scalarField(scalarType field) const
    {
        return _scalarFields[field];
    }
    double& Fields::scalarField(scalarType field, const mesh::Cell* cell)
    {
        return scalarField(field)(cell->index());
    }
    double Fields::scalarField(scalarType field, const mesh::Cell* cell) const
    {
        return scalarField(field)(cell->index());
    }
    Eigen::VectorX<vector2d>& Fields::vectorialField(scalarType field)
    {
        return _vectorialFields[field];
    }
    const Eigen::VectorX<vector2d>& Fields::vectorialField(scalarType field) const
    {
        return _vectorialFields[field];
    }

    vector2d& Fields::vectorialField(scalarType field, const mesh::Cell* cell)
    {
        return vectorialField(field)(cell->index());
    }

    const vector2d& Fields::vectorialField(scalarType field,const mesh::Cell* cell) const
    {
        return vectorialField(field)(cell->index());
    }

    Eigen::VectorX<vector2d>& Fields::velocityField()
    {
        return _velocity;
    }

    const Eigen::VectorX<vector2d>& Fields::velocityField() const
    {
        return _velocity;
    }

    vector2d& Fields::velocityField(const mesh::Face* face)
    {
        return velocityField()(face->index());
    }

    const vector2d& Fields::velocityField(const mesh::Face* face) const
    {
        return velocityField()(face->index());
    }
    
   
}
