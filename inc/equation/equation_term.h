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
#include <field/fields.h>

namespace term
{
    
    /**
     * Abstract class representing a equation term calculated over a face (surface integral).
     */
    class FaceEquationTerm
    {
    public:
        FaceEquationTerm(){}
        /**
         * Pure virtual function that is used to calculate equation term over a face.
         * It calculates the coefficients for cell1 and obtains the cell2 coeficient applying -1 factor.
         * 
         * \param submatrix
         * \param face
         * \param fields
         */
        virtual void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields) const = 0;

        /**
         * Pure virtual function that is used to calculate equation term over a face.
         * It calculates the coefficients for cell1 if isBoundary cell is false, otherwise calculates for cell2.
         *
         * \param submatrix
         * \param face
         * \param fields
         * \param isBoundaryCell if the cell you want to compute the term is a boundary cell or not.
         */
        virtual void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields, bool isBoundaryCell = false) const = 0;
    };
    /**
     * Abstract class representing a equation term calculated over a cell (volume integral).
     */
    class CellEquationTerm
    {
    public:
        CellEquationTerm() {}
        /**
         * Pure virtual function that is used to calculate the equation term over a cell .
         * 
         * \param submatrix
         * \param cell
         * \param fields
         */
        virtual void calculateCell(math::SystemSubmatrix* submatrix, const mesh::Cell* cell, const field::Fields* fields) const = 0;

    };
}
