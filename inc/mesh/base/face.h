/*****************************************************************//**
 * \file   Face.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <array>

//CFD
#include <mesh/base/node.h>
#include <base/global_typedef.h>
#include <mesh/base/complex_geometry.h>

#include <math/gradient.h>

namespace mesh
{

class Cell;

/**
 * @brief Represents a boundary between Cells or between a Cell and the boundary.
 * For memory optimization only 1 copy of each Face is created, and a pointer
 * is shared between adjacent Cells. Faces also store a reference to the cells
 * that is boundary of. Further implementation of duplicated faces to 
 * increse speed over memory may take place. Only available for 2D cell.
 * If face have only 1 cell when build funtion is called it is considered boundary.
 * Cell 1 can't be null, only Cell 2 can be null in case its boundary.
 */
class Face : public ComplexGeometry
{
public:

    //----PUBLIC CONSTRS------//
    Face();



    //------SETTERS-----------//
    void setNodes(Node* _node1_, Node* _node2_);
    void setCells(Cell* _cell1_, Cell* _cell2_);
    void setCell1(Cell* _cell1_);
    void setCell2(Cell* _cell2_);
    void setIndex(size_t _index_);

    //-------GETTERS---------//
    size_t index() const { return _index; }
    vector2d getNormal(const Cell* cell) const;
    double getUnitFlux(const Cell* cell) const;
    vector2d getCentroid() const;
    Cell* getOtherCell(const Cell* this_cell) const;
    inline bool hasCell1() const { return _cell1 != nullptr; }
    inline bool hasCell2() const { return _cell2 != nullptr; }
    inline const Node* node1() const { return _node1; }
    inline const Node* node2() const { return _node2; }
    inline bool isBoundary() const { return _isBoundary; }

    inline double area() const { return _area; }

    //------PUBLIC METHODS----------//
    void build();
    bool isEqual(Face* other) {
        return (_node1 == other->node1() && _node2 == other->node2()) 
            || (_node1 == other->node2() && _node2 == other->node1());
    }
    void updateUnitFlux(math::GradientScheme* scheme);

    ~Face()
    {
        if (env::show_debug_print__)
        {
            std::cout << "Face deleted" << std::endl;
        }
    }
private:
    size_t _index;
    bool _isBoundary = true;
    

    //Geometry info
    Node* _node1;
    Node* _node2;

    //Connectivity info
    Cell* _cell1;
    Cell* _cell2;

    //----Geometric properties----//
    //Vector
    vector2d _normal1;  //Normal vector of face element 1
    vector2d _normal2;  //Normal vector of face element 2
    //Points
    vector2d _centroid; //Face centroid coordinates (x[m],y[m])
    //Distance
    double _cell_centroid_distance; //Distance [m] between cell centers connected by this face hi+1-hi;

    //Area
    double _area; //FACE AREA [m^2]: Equals to lenght of the face * 1[m] in 2D

    //FLUX
    double _unitFlux1=0;
    double _unitFlux2=0;
    //-----PRIVATE METHODS-----//
    vector2d _calculateNormalVector(const Cell* cell);
    void _updateNormals();
    void _updateCentroid();
    void _updateArea();
    
    


};


}