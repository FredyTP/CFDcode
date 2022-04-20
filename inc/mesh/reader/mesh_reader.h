#pragma once
/*****************************************************************//**
 * \file   mesh_reader.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

#include <fstream>
#include <string>


#include <mesh/manipulation/mesh_selection.h>

namespace mesh
{
    namespace reader
    {
        template<class mesh_element>
        class MeshElementReader
        {
        public:
            MeshElementReader(){}
            
            virtual void read(const mesh::Mesh* mesh, std::vector<mesh_element*>& elements) = 0;
        };
        class FaceReader : public MeshElementReader<Face>
        {
        public:           
            FaceReader(std::string _path_) :_path(_path_)
            {

            }
            void read(const mesh::Mesh* mesh, std::vector<mesh::Face*>& elements)
            {
                std::ifstream file;
                file.open(_path);
                if (file.is_open())
                {
                    std::string bcstring;
                    std::getline(file, bcstring);

                    std::vector<size_t> node_ids;
                    std::string node_id_string;
                    std::istringstream stream(bcstring);


                    while (std::getline(stream, node_id_string, '\t'))
                    {
                        node_ids.push_back(std::stoi(node_id_string) - 1);
                    }
                    _setFacesFromNodes(mesh, node_ids, elements);
                    
                }
            }
        private:
            void _setFacesFromNodes(const mesh::Mesh* mesh_, std::vector<size_t>& node_ids_, std::vector<mesh::Face*>& faces)
            {
                for (size_t i = 0; i < node_ids_.size() - 1; i++)
                {
                    mesh::Face tempface;
                    size_t node1idx = node_ids_[i];
                    size_t node2idx = node_ids_[i + 1];
                    tempface.setNodes(mesh_->nodes()->at(node1idx).get(), mesh_->nodes()->at(node2idx).get());
                    for (size_t faceid = 0; faceid < mesh_->faces()->size(); faceid++)
                    {
                        mesh::Face* addface = mesh_->faces()->at(faceid).get();
                        if (tempface.isEqual(addface))
                        {
                            faces.push_back(addface);
                        }
                    }
                }
            }
            std::string _path;
              
        };
    }
}
