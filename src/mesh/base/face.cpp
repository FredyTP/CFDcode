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

Face::Face(Node* _node1_, Node* _node2_) :_node1(_node1_), _node2(_node2_)
{
    _cell1 = nullptr;
    _cell2 = nullptr;
}

Face::Face(Node* _node1_, Node* _node2_, Cell* _cell1_, Cell* _cell2_) :_node1(_node1_), _node2(_node2_), _cell1(_cell1_), _cell2(_cell2_)
{

}

void Face::setNodes(Node* _node1_, Node* _node2_)
{
    ComplexGeometry::modified();
    _node1 = _node1_;
    _node2 = _node2_;

}

void Face::setCells(Cell* _cell1_, Cell* _cell2_)
{
    ComplexGeometry::modified();
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

vector2d Face::getNormal(Cell* cell) const
{
    return cell == _cell1 ? _normal1 : _normal2;
}

Cell* Face::getOtherCell(Cell* this_cell) const
{
    return this_cell == _cell1 ? _cell2 : _cell1;
}

void Face::build()
{
    ComplexGeometry::build();
    _isBoundary = _cell2 == nullptr;
    this->_updateCentroid();
    this->_updateNormals();
}

void Face::_updateNormals()
{
   
    _normal1 = this->_calculateNormalVector(_cell1);
    if (this->hasCell2()) //If not at boundary
    {
        _normal2 = -_normal1;
    }
   
}

void Face::_updateCentroid()
{
    if (_node1 != nullptr && _node2 != nullptr)
    {
        _centroid = (_node1->pos() + _node2->pos()) / 2;
    }
}

vector2d Face::_calculateNormalVector(const Cell* cell)
{

    double nx, ny;
    vector2d cell_to_face = _centroid - cell->getCentroid();;
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
