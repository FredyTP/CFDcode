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
    Cell();
    Cell(std::vector<Node*> _nodes_, std::vector<Face*> _faces_);

    //-------SETTERS--------//
    void setNodes(std::vector<Node*> _nodes_);
    void setFaces(std::vector<Face*> _faces_);

    //-------GETTERS-------//
    vector2d getCentroid() const { return _centroid; }
    const std::vector<Node*>& nodes() { return _nodes; }
    const std::vector<Face*>& faces() { return _faces; }
    double volume() { return _volume; }


    void build();

    //----DESTRUCTOR----//
    ~Cell();
private:
    std::vector<Node*> _nodes;
    std::vector<Face*> _faces;
    
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

