/*****************************************************************//**
 * \file   interpolation.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <vector>
#include <base/global_typedef.h>

#include <math/value/cell_value.h>
#include <math/value/face_value.h>
#include <math/value/node_value.h>


namespace math
{
class GradientInterpolation
{
public:
    static void GreenGaussGradient(const mesh::Cell* cell, std::vector<FaceValue<vector2d>>& facevalues)
    {
        double volume = cell->volume();
        for (auto face : cell->faces())
        {
            facevalues.push_back(FaceValue<vector2d>(face, face->area() / volume * face->getNormal(cell)));
        }

    }
};
    /**
     * Class containing different interpolation schemes to interpolate
     * field values in the cell face
     */
class FaceInterpolation
{
public:
    /**
     * \brief Computes the Upwind Differencing Scheme(UDS) interpolation
     * on the cell's face. 
     * 
     * Properties:
     * - First order
     * - Unconditionallly stable(no oscillations)
     * - Numerically diffusive
     * 
     * \param vel_dot_normal: Dot product of vel on face with normal vector to the face (pointing outside the cell)
     * \param cell_coef: Value at the cell center
     * \param cell_neig_coef: Value at the cell neighbour connected by the face
     */
    static void UpwindDifferencingScheme(const mesh::Cell* cell, const mesh::Face* face,
        const double vel_dot_normal, std::vector<CellValue<double>>& cellvalues)
    {
        if (vel_dot_normal > 0)
        {
            cellvalues.push_back(CellValue<double>(cell, 1));
        }
        else
        {
            cellvalues.push_back(CellValue<double>(face->getOtherCell(cell), 1));
        }
    }

    /**
     * \brief Computes the Central Differencing Scheme (CDS) interpolation
     * on the cell's face. 
     * 
     * \param lamba relation of cell distance to face divided to distance
     * of centroid's cells connected bu the face
     * \param phi_cell value of phi of the cell
     * \param phi_neig value of phi of neightbour cell
     * \return 
     */
    static void CentralDifferencingScheme(const mesh::Cell* cell, const mesh::Face* face, 
        const double lambda, std::vector<CellValue<double>>& cellvalues) 
    {       
        cellvalues.push_back(CellValue<double>(cell, 1 - lambda));
        cellvalues.push_back(CellValue<double>(face->getOtherCell(cell), lambda));
    }

/**
 * .
 * 
 * \param cell
 * \param face
 * \param peclet_number
 * \param lambda
 * \param cellvalues
 */
    static void PowerLaw(const mesh::Cell* cell, const mesh::Face* face,
        const double peclet_number,const double lambda, std::vector<CellValue<double>>& cellvalues)
    {
        double peclet_abs = abs(peclet_number);
        if (isinf(peclet_number) || isnan(peclet_number))
        {
            FaceInterpolation::UpwindDifferencingScheme(cell, face, peclet_number, cellvalues);
        }
        else if (peclet_abs < 1e-5 )
        {          
            FaceInterpolation::CentralDifferencingScheme(cell, face, lambda, cellvalues);
        }
        else
        {
            double peclet_val = (exp(peclet_number * lambda) - 1) / (exp(peclet_number) - 1);
            FaceInterpolation::CentralDifferencingScheme(cell, face, peclet_val, cellvalues);
        }
    }

    static void SecondOrderUpwing(const mesh::Cell* cell, const mesh::Face* face, 
        double vel_dot_normal, std::vector<CellValue<double>>& cellvalues)
    {
        const mesh::Cell* upstream_cell;
        //Create function for getting upstream cell
        if (vel_dot_normal > 0)
        {
            upstream_cell = cell;
        }
        else
        {
            upstream_cell = face->getOtherCell(cell);
        }
        bool hasBoundary = false;
        for (auto upface : upstream_cell->faces())
        {
            if (upface->isBoundary())
            {
                hasBoundary = true;
            }
        }
        if (hasBoundary)
        {
            UpwindDifferencingScheme(cell, face, vel_dot_normal, cellvalues);
        }
        else
        {
            

            //Provide this as parameter :)
            std::vector<FaceValue<vector2d>> facevalues;
            GradientInterpolation::GreenGaussGradient(upstream_cell, facevalues);

            //This should be separated function
            vector2d celltoface = (face->getCentroid() - upstream_cell->getCentroid());
            CellValue<double> center_cell(upstream_cell, 1);
            for (int i = 0; i < facevalues.size(); i++)
            {
                double dotprod = celltoface.dot(facevalues[i].coef);
                center_cell.coef += (dotprod / 2);
                cellvalues.push_back(CellValue<double>(facevalues[i].face->getOtherCell(upstream_cell), dotprod / 2));
            }
            cellvalues.push_back(center_cell);
        }
        
    }
    //UNIMPLEMENTED INTERPOLATION METHODS
    //template<class type> static type quick()=0;
    //template<class type> static type power_law() = 0;
};

class GradientFluxInterpolation
{
    /**
     * .
     * 
     * \param pos1
     * \param pos2
     * \param facenormal
     * \param area
     * \param cell_coef
     * \param cell_neig_coef
     */
    static void SimpleGradientFlux(const vector2d& pos1, const vector2d& pos2, const vector2d facenormal,double area,
        double& cell_coef, double& cell_neig_coef)
    {
        vector2d r21 = (pos2 - pos1);
        double flux = area * r21.dot(facenormal) / r21.squaredNorm();
        cell_neig_coef = flux;
        cell_coef = -flux;
    }
};



}

