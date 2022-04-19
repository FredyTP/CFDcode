#include <mesh/base/cell.h>

#include <iostream> //Temporal for cout

namespace mesh {



void Cell::setNodes(std::vector<Node*> _nodes_)
{
    BaseGeometry::modified();
    _nodes = _nodes_;
}

void Cell::setFaces(std::vector<Face*> _faces_)
{
    BaseGeometry::modified();
    _faces = _faces_;
}

void Cell::setMaterial(material::Material* _material_)
{
    _material = _material_;
}

void Cell::_updateCentroid()
{
    size_t n_nodes = _nodes.size();
    _centroid.setZero();
    for (size_t i = 0; i < n_nodes; i++)
    {
        _centroid += _nodes[i]->pos();
    }
    _centroid /= n_nodes;
}

void Cell::_updateVolume()
{
    _volume = 0.0;

    size_t j = _nodes.size() - 1;
    for (size_t i = 0; i < _nodes.size(); i++)
    {
        _volume += (_nodes.at(j)->pos().x() + _nodes.at(i)->pos().x()) * (_nodes.at(j)->pos().y() - _nodes.at(i)->pos().y());
        j = i;
    }
    _volume= abs(_volume / 2.0);

}

void Cell::build()
{
    BaseGeometry::build();
    this->_updateCentroid();
    this->_updateVolume();
}

Cell::~Cell()
{

    if (env::show_debug_print__)
    {
        std::cout << "Cell deleted" << std::endl;
    }
}

}