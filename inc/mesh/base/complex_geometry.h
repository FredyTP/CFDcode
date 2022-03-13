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


class ComplexGeometry
{
public:
    virtual bool isBuild() final { return _build; }
protected:
    virtual void build() { _build = true; }
    virtual void modified() final { _build = false; }
private:
    bool _build = false;

};


}