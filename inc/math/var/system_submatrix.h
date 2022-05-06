#pragma once
/*****************************************************************//**
 * \file   system_submatrix.h
 * \brief
 *
 *
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once


 //STL
#include <list>

//CFD
#include <math/value/cell_value.h>

namespace math
{
    /**
     * This class handles the building of the system of equations.
     */
    class SystemSubmatrix
    {
    public:
        SystemSubmatrix(int size)
        {
            _cellCoeficients = std::make_unique< std::list<Eigen::Triplet<double>>>();
            _cellDotCoeficients = std::make_unique< std::list<Eigen::Triplet<double>>>();
            _coeficients = std::make_unique<Eigen::VectorXd>();
            _coeficients->resize(size);
            _coeficients->setZero();
        }
        /**
         * Move the saved data into another class.
         * 
         * \param other
         */
        void moveInto(SystemSubmatrix* other)
        {
            other->_cellCoeficients = std::move(_cellCoeficients);
            other->_cellDotCoeficients = std::move(_cellDotCoeficients);
            other->_coeficients = std::move(_coeficients);
        }
        int index = 0;

        /**
         * Add the cell value to the both cells containing that face. Handles the sign change of the opposite cell
         * 
         * \param face
         * \param cellvalue
         */
        void addFaceValues(const mesh::Face* face, const CellValue<double>& cellvalue)
        {
            if (cellvalue.cell != nullptr)
            {
                _cellCoeficients->push_back(Eigen::Triplet<double>(face->cell1()->index(), cellvalue.cell->index(), cellvalue.coef));
                _cellCoeficients->push_back(Eigen::Triplet<double>(face->cell2()->index(), cellvalue.cell->index(), -cellvalue.coef));
            }
            else
            {
                (* _coeficients.get())(face->cell1()->index()) += cellvalue.coef;
                (*_coeficients.get())(face->cell2()->index()) -= cellvalue.coef;
            }
        }

        /**
         * Add a list of cell values to both cells containing the face.
         * 
         * \param face
         * \param cellvalues
         */
        void addFaceValues(const mesh::Face* face, const std::vector<CellValue<double>>& cellvalues)
        {
            for (auto& cellvalue : cellvalues)
            {
                addFaceValues(face, cellvalue);
            }          
        }

        /**
         * Add a cell value to only one cell.
         */

        void addCellValues(const mesh::Cell* cell, const CellValue<double>& cellvalue, bool invert = false)
        {
            double coef = cellvalue.coef;
            if (invert) coef *= -1;
            if (cellvalue.cell != nullptr)
            {
                _cellCoeficients->push_back(Eigen::Triplet<double>(cell->index(), cellvalue.cell->index(), coef));
            }
            else
            {
                (*_coeficients.get())(cell->index()) += coef;
            }
        }

        /**
         * Add cell dot values(time derivative) to only one cell.
         * 
         * \param cell
         * \param cellvalue
         * \param invert
         */
        void addCellDotValues(const mesh::Cell* cell, const CellValue<double>& cellvalue, bool invert = false)
        {
            double coef = cellvalue.coef;
            if (invert) coef *= -1;
            if (cellvalue.cell != nullptr)
            {
                _cellDotCoeficients->push_back(Eigen::Triplet<double>(cell->index(), cellvalue.cell->index(), coef));
            }
            else
            {
                (*_coeficients.get())(cell->index()) += coef;
            }    
        }

        /**
         * Add a list of cell values to only one cell.
         * 
         * \param cell
         * \param cellvalues
         * \param invert
         */
        void addCellValues(const mesh::Cell* cell, const std::vector<CellValue<double>>& cellvalues, bool invert = false)
        {
            for (auto& cellvalue : cellvalues)
            {
                addCellValues(cell, cellvalue, invert);
            }

        }

        /**
         * Add a list of cell dot values to only one cell.
         * 
         * \param cell
         * \param cellvalues
         * \param invert
         */
        void addCellDotValues(const mesh::Cell* cell, const std::vector<CellValue<double>>& cellvalues, bool invert = false)
        {
            for (auto& cellvalue : cellvalues)
            {
                addCellDotValues(cell, cellvalue, invert);
            }

        }

        std::list<Eigen::Triplet<double>>* cellCoeficients()
        {
            return _cellCoeficients.get();
        }
        std::list<Eigen::Triplet<double>>* cellDotCoeficients()
        {
            return _cellDotCoeficients.get();
        }
        Eigen::VectorXd* coeficients()
        {
            return _coeficients.get();
        }
    private:
        std::unique_ptr<std::list<Eigen::Triplet<double>>> _cellCoeficients;
        std::unique_ptr<std::list<Eigen::Triplet<double>>> _cellDotCoeficients;
        std::unique_ptr<Eigen::VectorXd> _coeficients;
    };
}
