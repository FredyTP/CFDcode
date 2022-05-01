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
        void initfile(const std::string& filename)
        {
            std::ofstream file;
            _filename = filename + _extension;
            _contour_filename = filename + _imgExtension;
            file.open(_filename, std::ios::trunc);
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
        void saveSnapshot(const Eigen::VectorXd& _snapshot, vector2d offset = vector2d(0, 0))
        {
            std::ofstream file;
            file.open(_filename, std::ios::app);
            if (file.is_open())
            {
                for (int i = 0; i < _snapshot.size(); i++)
                {
                    auto cell = _problem->mesh()->cells()->at(i).get();
                    if (cell->nodes().size() < 3)
                    {
                        break;
                    }
                    vector2d pos = _problem->mesh()->cells()->at(i)->getCentroid() + offset;

                    file << pos.x() << "," << pos.y() << "," << _snapshot(i);
                    int maxFace = _problem->mesh()->maxCellFaces();
                    int facenum = cell->nodes().size();
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
                        file << "," << node->pos().x() + offset.x();
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
                        file << "," << node->pos().y() + offset.y();
                    }
                    file << std::endl;
                }
                file.close();
            }
        }
        void save_contour(size_t colorbar_div, size_t Rx, size_t Ry)
        {
            std::cout << "Saving contour: " << _filename << " to: " << _contour_filename << " ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('" + _filename + "','" + _contour_filename + "'," + std::to_string(colorbar_div) + "'," + std::to_string(Rx) + "," + std::to_string(Ry) + ")\"";
            system(command.c_str());
            std::cout << "Saved contour" << std::endl;
            std::string openimg = "\"" + _contour_filename + "\"";
            system(openimg.c_str());
        }
        void saveContourFile(std::string filename,Eigen::VectorXd* solution, bool saveJPG)
        {
            this->initfile(filename);
            this->saveSnapshot(*solution);
            if(saveJPG) this->save_contour(7, 1920, 1080);
      }
    private:
        std::string _filename;
        std::string _contour_filename;
        const std::string _extension = ".datctr";
        const std::string _imgExtension = ".png";
        sys::Problem* _problem = nullptr;
       
    };
}
