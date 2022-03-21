/*****************************************************************//**
 * \file   Cell.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#pragma once
//STL
#include <vector>

//CFD
#include <base/global_typedef.h>

#include <mesh/base/complex_geometry.h>
#include <mesh/base/node.h>
#include <mesh/base/face.h>
#include <material/material.h>

namespace mesh
{
    /**
     * TO SOLVE: how to make mesh independent of the state vector!
     */
   
/**
* @brief This class represents
* 2D Cells, 
* other types may be implemented further
*/
class Cell : public ComplexGeometry
{
public:
    //----PUBLIC CONSTRS----//
    Cell(size_t _index_) : _index(_index_) {};
   

    //-------SETTERS--------//
    void setNodes(std::vector<Node*> _nodes_);
    void setFaces(std::vector<Face*> _faces_);
    void setMaterial(material::Material* _material_);
    //-------GETTERS-------//
    vector2d getCentroid() const { return _centroid; }
    const std::vector<Node*>& nodes() const { return _nodes; }
    const std::vector<Face*>& faces() const { return _faces; }
    double volume() const { return _volume; }
    size_t index() const{ return _index; }
    const material::Material* material() const { return _material; }

    void build();

    //----DESTRUCTOR----//
    ~Cell();
private:
    size_t _index;
    std::vector<Node*> _nodes;
    std::vector<Face*> _faces;
    
    material::Material* _material;

    //----Geometric properties----//

    //Points
    vector2d _centroid;
    //Volume
    double _volume; //Cell volume [m^3]: equals to cell area *1[m] in 2D

    //------PRIVATE METHODS-----//
    void _updateCentroid();
    void _updateVolume();

};



}

