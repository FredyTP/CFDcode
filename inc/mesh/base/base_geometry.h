/*****************************************************************//**
 * \file   complex_geometry.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

namespace mesh
{

/**
 * Class representing base geometry for mesh elements.
 */
class BaseGeometry
{
    
public:
    virtual bool isBuild() final { return _build; }
    void setIndex(size_t _index_) { _index = _index_; }
    size_t index() const { return _index; }
    const vector2d& position() const { return _pos; }
protected:
    BaseGeometry() {}
    BaseGeometry(size_t _index_) :_index(_index_) {}
    virtual void build() { _build = true; }
    virtual void modified() final { _build = false; }

    //Position or centroid coordinates
    // ( x[m] , y[m] )
    vector2d _pos;
private:
    bool _build = false;
    size_t _index = -1;
    

};


}