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
#include <mesh/base/base_geometry.h>

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
class Face : public BaseGeometry
{
public:

    //----PUBLIC CONSTRS------//
    Face();


    //------SETTERS-----------//
    void setNodes(Node* _node1_, Node* _node2_);
    void setCells(Cell* _cell1_, Cell* _cell2_);
    void setCell1(Cell* _cell1_);
    void setCell2(Cell* _cell2_);
    

    //-------GETTERS---------//
    
    vector2d getNormal(const Cell* cell) const;
    vector2d getCentroid() const;
    Cell* getOtherCell(const Cell* this_cell) const;

    inline bool hasCell1() const { return _cell1 != nullptr; }
    inline bool hasCell2() const { return _cell2 != nullptr; }

    inline Cell* cell1() const { return _cell1; }
    inline Cell* cell2() const { return _cell2; }

    inline vector2d normal1() const { return _normal1; }
    inline vector2d normal2() const { return _normal2; }

    inline Node* node1() const { return _node1; }
    inline Node* node2() const { return _node2; }
    inline bool isBoundary() const { return _isBoundary; }

    inline double area() const { return _area; }
    inline double distanceCell2Cell() const { return _cell_centroid_distance; }
    inline double distanceToCell1() const { return _cell1_distance; }
    inline double lambda() const { return _lambda; }
    

    //------PUBLIC METHODS----------//
    void build();
    void rebuild();

    bool isEqual(Face* other) {
        return (_node1 == other->node1() && _node2 == other->node2()) 
            || (_node1 == other->node2() && _node2 == other->node1());
    }
   

    ~Face()
    {
        if (env::show_debug_print__)
        {
            std::cout << "Face deleted" << std::endl;
        }
    }
private:
    
    bool _isBoundary = true;
    

    //Geometry info
    Node* _node1;
    Node* _node2;


    //----Connectivity info----//
     
    //Cell 1 is the main cell of the face, all calculus are done
    //based on Cell1 and changed sign to get the Cell2 values.
    //Cell 2 contains the boundary cell in case this face is a boundary.
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
    double _cell1_distance;
    double _lambda;
    //Area
    double _area; //FACE AREA [m^2]: Equals to lenght of the face * 1[m] in 2D

    
    //-----PRIVATE METHODS-----//
    vector2d _calculateNormalVector(const Cell* cell);
    void _updateNormals();
    void _updateCentroid();
    void _updateArea();
    void _updateDistances();
    
    


};


}