/*****************************************************************//**
 * \file   boundary_condition.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <fstream>

//CFD
#include <mesh/base/face.h>
#include <field/field.h>
#include <math/convective/convective_term.h>
#include <math/var/system_submatrix.h>
#include <math/diffusive/diffusive_term.h>
namespace bc
{

class BoundaryCondition
{
public:
    BoundaryCondition() {}
    virtual void getBCCoefs(math::SystemSubmatrix *submatrix,const field::Fields* field) const
    {     
        for (auto face : _boundary)
        {          
             this->getBoundaryCondition(submatrix,face->getOtherCell(nullptr), face, field);            
        }       
    };
    virtual void getBoundaryCondition(math::SystemSubmatrix* submatrix,const mesh::Cell* cell, const  mesh::Face* face,const field::Fields* field) const = 0;
    void addFace(mesh::Face* _face_)
    {
        _boundary.push_back(_face_);
    }
    void loadBoundaryCondition(const std::string& path,const mesh::Mesh *mesh_)
    {
        std::ifstream file;
        file.open(path);
        if (file.is_open())
        {
            std::string bcstring;
            std::getline(file, bcstring);        

            std::vector<size_t> node_ids;
            std::string node_id_string;
            std::istringstream stream(bcstring);


            while (std::getline(stream, node_id_string, '\t'))
            {
                node_ids.push_back(std::stoi(node_id_string)-1);
            }
            this->setFacesFromNodes(mesh_, node_ids);
        }
    }
    void setFacesFromNodes(const mesh::Mesh* mesh_, std::vector<size_t>& node_ids_)
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
                    _boundary.push_back(addface);
                }
            }
        }
    }
    void setConvetiveTerm(math::convective::ConvectiveTerm* _convectiveTerm_)
    {
        _convectiveTerm = _convectiveTerm_;
    }
    void setDiffusiveTerm(math::diffusive::DiffusiveTerm* _diffusiveTerm_)
    {
        _diffusiveTerm = _diffusiveTerm_;
    }
protected:
    std::vector<mesh::Face*> _boundary;
    math::convective::ConvectiveTerm* _convectiveTerm = nullptr;
    math::diffusive::DiffusiveTerm* _diffusiveTerm = nullptr;
};

}
