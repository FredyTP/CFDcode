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
            std::string img_name = name + ".png";
            this->initfile(file_name);
            this->save(file_name, *_solution, _problem->mesh());
            if(saveJPG) this->save_contour(file_name, img_name,7, 1920, 1080);            
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
                file << "X" << "," << "Y" << "," << "Value";
                for (int i = 0; i < _problem->mesh()->maxCellFaces(); i++)
                {

                    file << "," << "VX" << i << "," << "VY" << i;
                }
                file << std::endl;
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
                    vector2d pos = pMesh->cells()->at(i)->getCentroid();

                    file << pos.x() << "," << pos.y() << "," << _solution(i);
                    int maxFace = pMesh->maxCellFaces();
                    int facenum = cell->nodes().size();
                    for (int i =0; i<maxFace ;i++)
                    {
                        mesh::Node* node;
                        if (i >= facenum)
                        {
                            node = cell->nodes()[facenum - 1];
                        }
                        else
                        {
                            node = cell->nodes()[i];
                        }
                        file << "," << node->pos().x();
                    }
                    for (int i = 0; i < maxFace; i++)
                    {
                        mesh::Node* node;
                        if (i >= facenum)
                        {
                            node = cell->nodes()[facenum - 1];
                        }
                        else
                        {
                            node = cell->nodes()[i];
                        }
                        file << "," << node->pos().y();
                    }
                    file<< std::endl;
                }
                file.close();
            }
        }
        void save_contour(const std::string& filename, const std::string& contour_filename, size_t colorbar_div, size_t Rx, size_t Ry)
        {
            std::cout << "Saving: " << filename << " to: " << contour_filename << " ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('" + filename + "','" + contour_filename + "'," + std::to_string(colorbar_div) + "'," + std::to_string(Rx) + "," + std::to_string(Ry) + ")\"";
            system(command.c_str());
            std::cout << "saved" << std::endl;
            std::string openimg = "\"" + contour_filename + "\"";
            system(openimg.c_str());
        }

    };
}
