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
#include <vector>

//CFD
#include <math/value/cell_value.h>

namespace math
{
    class SystemSubmatrixCoupled
    {
    public:
        SystemSubmatrixCoupled(int size,int n_var)
        {
            _coeficients.resize(size * n_var);
            _coeficients.setZero();
        }

        void addFaceValues(int vareq,int var,const mesh::Face* face, const CellValue<double>& cellvalue)
        {
            if (cellvalue.cell != nullptr)
            {
                _cellCoeficients.push_back(Eigen::Triplet<double>(vareq * face->cell1()->index(), var * cellvalue.cell->index(), cellvalue.coef));
                
                _cellCoeficients.push_back(Eigen::Triplet<double>(vareq * face->cell2()->index(), var * cellvalue.cell->index(), -cellvalue.coef));
            }
            else
            {
                _coeficients(vareq * face->cell1()->index()) += cellvalue.coef;
                _coeficients(vareq * face->cell2()->index()) -= cellvalue.coef;
            }
        }
        void addFaceValues(int vareq,int var,const mesh::Face* face, const std::vector<CellValue<double>>& cellvalues)
        {
            for (auto& cellvalue : cellvalues)
            {
                addFaceValues(vareq, var, face, cellvalue);
            }          
        }

        void addCellValues(int vareq,int var,const mesh::Cell* cell, const CellValue<double>& cellvalue, bool invert = false)
        {
            double coef = cellvalue.coef;
            if (invert) coef *= -1;
            if (cellvalue.cell != nullptr)
            {
                _cellCoeficients.push_back(Eigen::Triplet<double>(vareq*cell->index(), var*cellvalue.cell->index(), coef));
            }
            else
            {
                _coeficients(vareq*cell->index()) += coef;
            }
        }
        void addCellDotValues(const mesh::Cell* cell, const CellValue<double>& cellvalue, bool invert = false)
        {
            double coef = cellvalue.coef;
            if (invert) coef *= -1;
            if (cellvalue.cell != nullptr)
            {
                _cellDotCoeficients.push_back(Eigen::Triplet<double>(cell->index(), cellvalue.cell->index(), coef));
            }
            else
            {
                _coeficients(cell->index()) += coef;
            }
        }
        void addCellValues(int vareq,int var,const mesh::Cell* cell, const std::vector<CellValue<double>>& cellvalues, bool invert = false)
        {
            for (auto& cellvalue : cellvalues)
            {
                addCellValues(vareq,var,cell, cellvalue, invert);
            }

        }
        void addCellDotValues(const mesh::Cell* cell, const std::vector<CellValue<double>>& cellvalues, bool invert = false)
        {
            for (auto& cellvalue : cellvalues)
            {
                addCellDotValues(cell, cellvalue, invert);
            }

        }

        std::vector<Eigen::Triplet<double>>& cellCoeficients()
        {
            return _cellCoeficients;
        }
        std::vector<Eigen::Triplet<double>>& cellDotCoeficients()
        {
            return _cellDotCoeficients;
        }
        Eigen::VectorXd& coeficients()
        {
            return _coeficients;
        }
    private:
        std::vector<Eigen::Triplet<double>> _cellCoeficients;
        std::vector<Eigen::Triplet<double>> _cellDotCoeficients;
        Eigen::VectorXd _coeficients;
    };
}
