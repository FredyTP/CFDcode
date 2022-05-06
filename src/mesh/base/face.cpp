#include <mesh/base/face.h>

#include <mesh/base/cell.h> //for cell data access


namespace mesh{





Face::Face()
{
    if (env::show_debug_print__)
    {
        std::cout << "Created Face" << std::endl;
    }
    _node1 = nullptr;
    _node2 = nullptr;
    _cell1 = nullptr;
    _cell2 = nullptr;
}

void Face::setNodes(Node* _node1_, Node* _node2_)
{
    BaseGeometry::modified();
    _node1 = _node1_;
    _node2 = _node2_;

}

void Face::setCells(Cell* _cell1_, Cell* _cell2_)
{
    BaseGeometry::modified();
    _cell1 = _cell1_;
    _cell2 = _cell2_;
}

void Face::setCell1(Cell* _cell1_)
{
    _cell1 = _cell1_;
}

void Face::setCell2(Cell* _cell2_)
{
    _cell2 = _cell2_;
}

vector2d Face::getNormal(const Cell* cell) const
{
    return cell == _cell1 ? _normal1 : _normal2;
}

vector2d Face::getCentroid() const
{
    return _pos;
}

Cell* Face::getOtherCell(const Cell* this_cell) const
{
    return this_cell == _cell1 ? _cell2 : _cell1;
}

void Face::build()
{
    BaseGeometry::build();
    _isBoundary = _cell2 == nullptr;
    this->_updateCentroid();
    this->_updateNormals();
    this->_updateArea();
    this->_updateDistances();
}
void Face::rebuild()
{
    BaseGeometry::build();
    this->_updateCentroid();
    this->_updateNormals();
    this->_updateArea();
    this->_updateDistances();
}


void Face::_updateNormals()
{
   
    _normal1 = this->_calculateNormalVector(_cell1);
    _normal2 = -_normal1;

   
}

void Face::_updateCentroid()
{
    if (_node1 != nullptr && _node2 != nullptr)
    {
        _pos = (_node1->pos() + _node2->pos()) / 2;
    }
}

void Face::_updateArea()
{
    _area = (_node2->pos() - _node1->pos()).norm();
}

void Face::_updateDistances()
{
    if (hasCell2())
    {
        vector2d cell2cell = cell2()->getCentroid() - cell1()->getCentroid();
        _cell_centroid_distance = cell2cell.norm();
        vector2d face2cell = cell2()->getCentroid() - getCentroid();
        vector2d cell2face = getCentroid() - cell1()->getCentroid();
        _cell1_distance = cell2face.norm();
        _lambda = _cell1_distance / (_cell_centroid_distance+face2cell.norm());
    }

}

vector2d Face::_calculateNormalVector(const Cell* cell)
{

    double nx, ny;
    vector2d cell_to_face = _pos - cell->getCentroid();;
    vector2d pA = _node1->pos();
    vector2d pB = _node2->pos();
    nx = pA[1] - pB[1];
    ny = pB[0] - pA[0];
    vector2d normal = { nx,ny };
    normal.normalize();

    double discriminant = cell_to_face.dot(normal);
    if (discriminant < 0)
    {
        normal *= -1;
    }
    return normal;
        
}


}
