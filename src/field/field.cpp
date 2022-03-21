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
    Field::Field(const mesh::Mesh* _mesh_)
    {
        _mesh = _mesh_;
        if (_mesh != nullptr)
        {
            _velocityField.resize(_mesh->faces()->size());
            _scalarFields.resize(_mesh->cells()->size());

        }

    }
}
