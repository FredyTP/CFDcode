/*****************************************************************//**
 * \file   gradient_interpolation.cpp
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <math/interpolation/gradient_interpolation.h>
#include <math/interpolation/face_interpolation.h>

namespace math
{
    void GradientInterpolation::FiniteDifferenceGradient(const mesh::Face* face, std::vector<CellValue<vector2d>>& cellvalues, double factor)
    {
        vector2d gradient = factor * (face->cell2()->getCentroid() - face->cell1()->getCentroid()) / pow(face->distanceCell2Cell(),2);
        cellvalues.push_back(CellValue<vector2d>(face->cell2(), gradient));
        cellvalues.push_back(CellValue<vector2d>(face->cell1(), -gradient));
       
    }
    void GradientInterpolation::GreenGaussGradient(const mesh::Cell* cell, std::vector<CellValue<vector2d>>& cellvalues, double factor)
    {
        double volume = cell->volume();
        for (auto face : cell->faces())
        {    

            FaceInterpolation::CentralDifferencingScheme<vector2d>(face, face->lambda(), cellvalues, face->area() * factor / volume * face->getNormal(cell));
        }

    }
    void GradientInterpolation::LeastSquaresGradient(const mesh::Cell* cell, std::vector<CellValue<vector2d>>& cellvalues, double factor)
    {
        Eigen::MatrixXd A = Eigen::MatrixXd::Zero(cell->faces().size(), 2);
        Eigen::VectorXd B = Eigen::VectorXd::Zero(cell->faces().size());
       
        for (int i =0;i<cell->faces().size();i++)
        {
            auto face = cell->faces()[i];
            double w = 1.0 / face->distanceCell2Cell();
            vector2d delta_pos = face->getOtherCell(cell)->position() - cell->position();
            A(i, 0) = w * delta_pos.x();
            A(i, 1) = w * delta_pos.y();
            B(i) = w;

        }
        
        vector2d solution = (A.transpose() * A).ldlt().solve(A.transpose() * B);
        
        for (int i = 0; i < cell->faces().size(); i++)
        {
            auto face = cell->faces()[i];           
            cellvalues.push_back(CellValue<vector2d>(face->getOtherCell(cell), factor * solution));
            cellvalues.push_back(CellValue<vector2d>(cell, -factor * solution));
        }
    }

    void GradientInterpolation::DotProduct(std::vector<CellValue<double>>& result,std::vector<CellValue<vector2d>>& cellvalues, vector2d vec)
    {
        for (auto& cell : cellvalues)
        {
            result.push_back(CellValue<double>(cell.cell, cell.coef.dot(vec)));
        }
    }
}
