/*****************************************************************//**
 * \file   temporal_solution_saver.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#pragma once
#include <string>
#include <field/fields.h>
#include <fstream>
#include <mesh/mesh.h>
#include <memory>
namespace post
{


    class TimeSolutionSaver
    {
    public:
        TimeSolutionSaver()
        {

        }

        void initFile(const std::string name, double spacingFactor = 0.05)
        {
            _filename = name;
            std::string dataname = _filename + ".tdat";
            _spacingFactor = spacingFactor;
            std::ofstream file;
            file.open(dataname, std::ios::trunc);
            if (file.is_open())
            {
                file << "X" << "," << "Y" << "," << "Value" << "," << "VX1" << "," << "VY1" << "," << "VX2" << "," << "VY2" << "," << "VX3" << "," << "VY3" << ","<<"t" << std::endl;
                file.close();
            }
        }
        void saveSnapshot(const field::Fields* fields, double actualTime)
        {
            std::ofstream file;
            auto pMesh = fields->mesh();
            std::string dataname = _filename + ".tdat";
            file.open(dataname, std::ios::app);
            if (file.is_open())
            {
                double deltax = pMesh->boundingBox().size.x() * (1 + _spacingFactor) * save_snapshot_step;
                
                for (int i = 0; i < fields->scalarField(field::temperature).size(); i++)
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

                    file << pos.x() + deltax << "," << pos.y() << "," << fields->scalarField(field::temperature)(i)
                        << "," << n0->pos().x() + deltax << "," << n1->pos().x() + deltax
                        << "," << n2->pos().x() + deltax << "," << n0->pos().y()
                        << "," << n1->pos().y() << "," << n2->pos().y() <<","<< actualTime << std::endl;
                }
                file.close();
                std::cout << "Saved time solution at: " << actualTime << "s in: " << dataname << std::endl;

            }
            save_snapshot_step++;
        }
        /*void addSnapshot(const field::Fields* fields, double actualTime)
        {
            mesh::Mesh* mesh=nullptr;
      
            _snapshots.push_back(SolutionSnapshot());
            _snapshots.back().fields = std::make_unique<field::Fields>(mesh);
            _snapshots.back().fields->copy(fields);
            _snapshots.back().actualTime = actualTime;
        }*/

        //const std::vector<SolutionSnapshot> snapshots() { return _snapshots; }

        void save_contour(size_t Rx, size_t Ry)
        {
            std::string dataname = _filename + ".tdat";
            std::string contour_filename = _filename + ".jpg";
            std::cout << "Saving: " << dataname << " to: " << contour_filename << " ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('" + dataname + "','" + contour_filename + "'," + std::to_string(Rx) + "," + std::to_string(Ry) + ")\"";
            system(command.c_str());
            std::cout << "saved" << std::endl;
            std::string openimg = "\"" + contour_filename + "\"";
            system(openimg.c_str());
        }
    private:
        int save_snapshot_step = 0;
        double _spacingFactor = 0.05;
        std::string _filename;
        //std::vector<SolutionSnapshot> _snapshots;
        
    };

}
