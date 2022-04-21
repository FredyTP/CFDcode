#pragma once
/*****************************************************************//**
 * \file   equation_term.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <mesh/base/face.h>
#include <math/var/system_submatrix.h>
#include <field/field.h>

namespace term
{
    
    class FaceEquationTerm
    {
    public:
        FaceEquationTerm(){}
        virtual void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field) const = 0;
        virtual void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* field, bool isBoundaryCell = false) const = 0;
    };
    class CellEquationTerm
    {
    public:
        CellEquationTerm() {}
        virtual void calculateCell(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* field) const = 0;

    };
}
