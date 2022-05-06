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
    /**
     * Class for saving results on a 2d line.
     */
    class PostProcessLine
    {
    public:
        PostProcessLine(vector2d origin, vector2d end)
            : _origin(origin), _end(end)
        {
            
        }

        virtual void sampleSolution(const sys::Problem* problem, Eigen::VectorXd& solution, bool gradientCorrection = false) = 0;     

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
        static double GradientCorrection(const vector2d& pos,const mesh::Cell* cell, const Eigen::VectorXd& solution)
        {
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
            return deltaT;
        }
        const static mesh::Cell* FindInsideCell(const vector2d& point, const sys::Problem* problem)
        {
            for(auto& cell: *problem->mesh()->cells())
            {
                if (PostProcessLine::IsPointInsideCell(point, cell.get()))
                {
                    return cell.get();
                }
            }
            return nullptr;
        }


        static bool IsPointInsideCell(const vector2d& point, const mesh::Cell* cell)
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
    protected:
        vector2d _origin;
        vector2d _end;

        std::vector<double> _distance;
        std::vector<double> _value;
    };

    class SampledLine : public PostProcessLine
    {
    public:
        SampledLine(vector2d origin, vector2d end, uint32_t samples)
            : PostProcessLine(origin,end), _samples(samples)
        {

        }

        void sampleSolution(const sys::Problem* problem, Eigen::VectorXd& solution, bool gradientCorrection = false)
        {
            vector2d dr = (_end - _origin) / (_samples - 1);
            for (int i = 0; i < _samples; i++)
            {
                vector2d deltaPos = i * dr;
                vector2d pos = _origin + deltaPos;
                const mesh::Cell* cell = PostProcessLine::FindInsideCell(pos, problem);
                if (cell != nullptr)
                {
                    _distance.push_back(deltaPos.norm());

                    //USE GRADIENT CORRECTION
                    double temp = solution(cell->index());
                    if (gradientCorrection)
                    {
                        temp += PostProcessLine::GradientCorrection(pos, cell, solution);
                    }

                    _value.push_back(temp);
                }
            }
        }

    private:
        double _samples;
    };

    class CellSampledLine : public PostProcessLine
    {
    public:
        CellSampledLine(vector2d origin, vector2d end, uint32_t deltar)
            : PostProcessLine(origin, end), _deltar(deltar)
        {

        }

        void sampleSolution(const sys::Problem* problem, Eigen::VectorXd& solution, bool gradientCorrection = false)
        {
            vector2d lineDirection = (_end - _origin).normalized();
            vector2d dr = lineDirection * _deltar;
            const mesh::Cell* lastcell = nullptr;
            int tries = 0;
            while (tries < 1000)
            {
                vector2d deltaPos = tries * dr;
                vector2d pos = _origin + deltaPos;
                const mesh::Cell* cell = PostProcessLine::FindInsideCell(pos, problem);
                if (cell != nullptr)
                {
                    double maxdotprod = -10000;
                    vector2d cellpos = cell->position();
                    mesh::Cell* max_cell = nullptr;
                    mesh::Face* max_face = nullptr;
                    for (auto face : cell->faces())
                    {

                        vector2d dir = (face->getOtherCell(cell)->position() - cellpos).normalized();
                        double dotprod = -lineDirection.dot(dir);
                        if (dotprod > maxdotprod)
                        {
                            maxdotprod = dotprod;
                            max_cell = face->getOtherCell(cell);
                            max_face = face;
                        }
                      

                    }

                    if (max_face!=nullptr && max_cell!=nullptr && max_face->isBoundary())
                    {
                        //IF BOUNDARY FACE DETECTED
                        _distance.push_back(0);

                        //USE GRADIENT CORRECTION
                        double temp = solution(max_cell->index());
                        _value.push_back(temp);

                        vector2d cell2cell = (cell->position() - max_cell->position());
                        double deltar = cell2cell.dot(lineDirection);
                        _distance.push_back(deltar + _distance.back());

                        temp = solution(cell->index());
                        _value.push_back(temp);
                    }
                    else
                    {
                        _distance.push_back(0);

                        double temp = solution(cell->index());
                        _value.push_back(temp);
                    }

                    
                    while (1)
                    {
                        double maxdotprod = -10000;
                        vector2d cellpos = cell->position();
                        mesh::Cell* max_cell = nullptr;
                        for (auto face : cell->faces())
                        {
                            vector2d dir = (face->getOtherCell(cell)->position() - cellpos).normalized();
                            double dotprod = lineDirection.dot(dir);
                            if (dotprod > maxdotprod)
                            {
                                maxdotprod = dotprod;
                                max_cell = face->getOtherCell(cell);
                            }
                        }
                        if (maxdotprod > 0)
                        {
                            vector2d cell2cell = (max_cell->position() - cellpos);
                            double deltar = cell2cell.dot(lineDirection);
                            _distance.push_back(deltar + _distance.back());
                            double temp = solution(max_cell->index());

                            _value.push_back(temp);
                            cell = max_cell;
                        }
                        else
                        {
                            tries = 1000;
                            break;
                        }
                    }


                }
                else
                {
                    tries++;
                }
            }
             
        }

    private:
        double _deltar;
    };

}
