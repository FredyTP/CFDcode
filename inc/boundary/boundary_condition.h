/*****************************************************************//**
 * \file   boundary_condition.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
#include <mesh/base/face.h>
#include <field/field.h>
#
namespace bc
{

class BoundaryCondition
{
public:
    BoundaryCondition() {}
    virtual std::pair<std::vector<Eigen::Triplet<double>>, Eigen::VectorXd> getBCCoefs(const mesh::Mesh* pmesh,field::Field* field) const
    {
        Eigen::VectorXd indep(pmesh->cells()->size());
        std::vector<Eigen::Triplet<double>> triplets;
        indep.setZero();
        for (auto face : _boundary)
        {
            size_t idx = face->getOtherCell(nullptr)->index();
            auto pair = this->getBoundaryCondition(face->getOtherCell(nullptr), face, field);
            
            indep(idx) += pair.second;
            for (auto triplet : pair.first)
            {
                triplets.push_back(triplet);
            }
        }
        return std::pair<std::vector<Eigen::Triplet<double>>, Eigen::VectorXd > (triplets, indep);
    };
    virtual std::pair<std::vector<Eigen::Triplet<double>>, double> getBoundaryCondition(const mesh::Cell* cell, const mesh::Face* face, field::Field* field) const = 0;
    void addFace(mesh::Face* _face_)
    {
        _boundary.push_back(_face_);
    }
    
private:
    std::vector<mesh::Face*> _boundary;
};

}
