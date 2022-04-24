/*****************************************************************//**
 * \file   contour.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#pragma once

#include <system/problem.h>

namespace post
{
    class Contour
    {
    public:
        Contour() {}

        void setProblem(sys::Problem* _problem_)
        {
            _problem = _problem_;
        }
        void setSolution(Eigen::VectorXd& _solution_)
        {
            _solution = &_solution_;
        }

        void saveContourFile(const std::string name, bool saveJPG)
        {
            std::string file_name = name + _extension;
            std::string img_name = name + ".jpg";
            this->initfile(file_name);
            this->save(file_name, *_solution, _problem->mesh());
            if(saveJPG) this->save_contour(file_name, img_name, 1920, 1080);            
        }
    private:
        const std::string _extension = ".datctr";
        sys::Problem* _problem = nullptr;
        Eigen::VectorXd* _solution = nullptr;

        void initfile(const std::string& filename)
        {
            std::ofstream file;
            file.open(filename, std::ios::trunc);
            if (file.is_open())
            {
                file << "X" << "," << "Y" << "," << "Value" << "," << "VX1" << "," << "VY1" << "," << "VX2" << "," << "VY2" << "," << "VX3" << "," << "VY3" << std::endl;
                file.close();
            }
        }
        void save(const std::string& filename, const Eigen::VectorXd& _solution, const mesh::Mesh* pMesh)
        {
            std::ofstream file;
            file.open(filename, std::ios::app);
            if (file.is_open())
            {
                for (int i = 0; i < _solution.size(); i++)
                {
                    auto cell = pMesh->cells()->at(i).get();
                    if (cell->nodes().size() < 3)
                    {
                        break;
                    }
                    auto n0 = cell->nodes().at(0);
                    auto n1 = cell->nodes().at(1);
                    auto n2 = cell->nodes().at(2);

                    vector2d pos = pMesh->cells()->at(i)->getCentroid();

                    file << pos.x() << "," << pos.y() << "," << _solution(i)
                        << "," << n0->pos().x() << "," << n1->pos().x() << "," << n2->pos().x()
                        << "," << n0->pos().y() << "," << n1->pos().y() << "," << n2->pos().y()
                        << std::endl;
                }
                file.close();
            }
        }
        void save_contour(const std::string& filename, const std::string& contour_filename, size_t Rx, size_t Ry)
        {
            std::cout << "Saving: " << filename << " to: " << contour_filename << " ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('" + filename + "','" + contour_filename + "'," + std::to_string(Rx) + "," + std::to_string(Ry) + ")\"";
            system(command.c_str());
            std::cout << "saved" << std::endl;
            std::string openimg = "\"" + contour_filename + "\"";
            system(openimg.c_str());
        }

    };
}
