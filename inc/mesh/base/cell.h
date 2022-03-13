/*****************************************************************//**
 * \file   Cell.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#pragma once
//STL
#include <array>

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
* 2D triangular Cells, 
* other types may be implemented further
*/
class Cell : public ComplexGeometry
{
public:
    //----PUBLIC CONSTRS----//
    Cell();
    Cell(std::array<Node*, 3> _nodes_, std::array<Face*, 3> _faces_);

    //-------SETTERS--------//
    void setNodes(std::array<Node*, 3> _nodes_);
    void setFaces(std::array<Face*, 3> _faces_);

    //-------GETTERS-------//
    vector2d getCentroid() const { return _centroid; }
    const std::array<Node*, 3>& nodes() { return _nodes; }
    const std::array<Face*, 3>& faces() { return _faces; }


    void build();

    //----DESTRUCTOR----//
    ~Cell();
private:
    std::array<Node*, 3> _nodes;
    std::array<Face*, 3> _faces;
    
    //----Geometric properties----//

    //Points
    vector2d _centroid;
    //Volume
    double _volume; //Cell volume [m^3]: equals to cell area *1[m] in 2D

    //------PRIVATE METHODS-----//
    void _updateCentroid();

};



}

