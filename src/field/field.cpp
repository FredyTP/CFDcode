/*****************************************************************//**
 * \file   field.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <field/field.h>

namespace field
{
    Fields::Fields(const mesh::Mesh* _mesh_)
    {
        _mesh = _mesh_;
        if (_mesh != nullptr)
        {
            //AT FACES
            _velocityField.resize(_mesh->faces()->size());

            //AT CELLS
            _densityField.resize(_mesh->cells()->size());
            _temperatureField.resize(_mesh->cells()->size());
            _pressureField.resize(_mesh->cells()->size());
        }

    }
    void Fields::copy(const Fields* other)
    {
        if (_mesh == other->_mesh)
        {
            _velocityField = other->_velocityField;
            _densityField = other->_densityField;
            _temperatureField = other->_temperatureField;
            _pressureField = other->_pressureField;
        }
    }
    const vector2d& Fields::velocityField(const mesh::Face* face) const
    {
        return _velocityField(face->index());
    }
    const ScalarStateVector& Fields::scalarField(const mesh::Cell* cell) const
    {
        return scalarField(cell->index());
    }
    const vector2d& Fields::velocityField(size_t index) const
    {
        return _velocityField(index);
    }
    const ScalarStateVector& Fields::scalarField(size_t index) const
    {
        return ScalarStateVector(_densityField(index),
            _temperatureField(index), _pressureField(index));
    }
}
