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


class BaseGeometry
{
    
public:
    virtual bool isBuild() final { return _build; }
    void setIndex(size_t _index_) { _index = _index_; }
    size_t index() const { return _index; }
protected:
    BaseGeometry() {}
    BaseGeometry(size_t _index_) :_index(_index_) {}
    virtual void build() { _build = true; }
    virtual void modified() final { _build = false; }
private:
    bool _build = false;
    size_t _index = -1;

};


}