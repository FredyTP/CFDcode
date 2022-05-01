/*****************************************************************//**
 * \file   sampled_line.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   May 2022
 *********************************************************************/
#include <base/global_typedef.h>
#include <mesh/base/cell.h>
#include <system/problem.h>
#include <math/interpolation/gradient_interpolation.h>
namespace post
{
    class SampledLine
    {
    public:
        SampledLine(vector2d origin, vector2d end, uint32_t samples)
            : _origin(origin), _end(end), _samples(samples)
        {
            
        }

        void sampleSolution(const sys::Problem* problem, Eigen::VectorXd& solution)
        {
            vector2d dr = (_end - _origin) / (_samples - 1);
            for (int i = 0; i < _samples; i++)
            {
                vector2d deltaPos = i * dr;
                vector2d pos = _origin + deltaPos;
                const mesh::Cell* cell = SampledLine::findInsideCell(pos, problem);
                if (cell != nullptr)
                {
                    _distance.push_back(deltaPos.norm());

                    //USE GRADIENT CORRECTION
                    double temp = solution(cell->index());
                    vector2d vect = pos - cell->position();
                    std::vector<math::CellValue<vector2d>> gradient;
                    std::vector<math::CellValue<double>> cellvalues;
                    math::GradientInterpolation::GreenGaussGradient(cell, gradient);
                    math::GradientInterpolation::DotProduct(cellvalues, gradient, vect);               
                    double deltaT = 0;
                    for (auto& val : cellvalues)
                    {
                        deltaT += val.coef * solution(val.cell->index());
                    }
                    temp += deltaT;
                    _value.push_back(temp);
                }
            }
        }

        void saveResults(const std::string& filename)
        {
            std::string file_name = filename + ".csv";
            std::ofstream file;
            file.open(file_name, std::ios::trunc);
            if (file.is_open())
            {
                file << "s [m]" << "," << "Value"<<std::endl;
                for (int i = 0; i < _distance.size(); i++)
                {

                    file <<_distance[i] << "," << _value[i]<< std::endl;
                }               
                file.close();
            }

        }
        const static mesh::Cell* findInsideCell(const vector2d& point, const sys::Problem* problem)
        {
            for(auto& cell: *problem->mesh()->cells())
            {
                if (SampledLine::isPointInsideCell(point, cell.get()))
                {
                    return cell.get();
                }
            }
            return nullptr;
        }


        static bool isPointInsideCell(const vector2d& point, const mesh::Cell* cell)
        {
            bool isInside = true;
            //Check if cell is a boundary cell
            //If it is, the point cant be inside
            if (cell->faces().size() == 1)
            {
                return false;
            }
            for(auto face : cell->faces())
            {
                vector2d p2f = face->position() - point;
                if (p2f.dot(face->getNormal(cell)) < 0)
                {
                    isInside = false;
                }
            }
            return isInside;
        }
    private:
        vector2d _origin;
        vector2d _end;
        double _samples;

        std::vector<double> _distance;
        std::vector<double> _value;
    };
}
