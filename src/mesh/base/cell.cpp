#include <mesh/base/cell.h>

#include <iostream> //Temporal for cout

namespace mesh {


Cell::Cell()
{
    if (env::show_debug_print__)
    {
        std::cout << "Created Cell" << std::endl;
    }

    _nodes = {};
    _faces = {};
}

Cell::Cell(std::array<Node*, 3> _nodes_, std::array<Face*, 3> _faces_) : _nodes(_nodes_), _faces(_faces_)
{
    this->build();
}

void Cell::setNodes(std::array<Node*, 3> _nodes_)
{
    ComplexGeometry::modified();
    _nodes = _nodes_;
}

void Cell::setFaces(std::array<Face*, 3> _faces_)
{
    ComplexGeometry::modified();
    _faces = _faces_;
}

void Cell::_updateCentroid()
{
    _centroid = (_nodes[0]->pos() + _nodes[1]->pos() + _nodes[2]->pos()) / 3;
}

void Cell::build()
{
    ComplexGeometry::build();
    this->_updateCentroid();
    //TODO: computeVolume
    //this->_updateVolume();
}

Cell::~Cell()
{

    if (env::show_debug_print__)
    {
        std::cout << "Cell deleted" << std::endl;
    }
}

}