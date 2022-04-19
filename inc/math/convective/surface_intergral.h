/*****************************************************************//**
 * \file   surface_intergral.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once


//STL
#include <vector>

//CFD
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

#include <math/var/face_variable.h>
#include <math/var/system_submatrix.h>

#include <field/field.h>
namespace math
{
namespace convective
{


class SurfaceIntegral
{
public:
    SurfaceIntegral()
    {

    }

    virtual std::vector<FaceVariable> integrate(double coef, const mesh::Face* face) = 0;
};

class MidPoint : public SurfaceIntegral
{
public:
    MidPoint()
    {

    }
    virtual std::vector<FaceVariable> integrate(double coef, const mesh::Face* face)
    {
        FaceVariable centerFace(face->getCentroid(), face, face->cell1()->index(), coef);
        return {centerFace};
    }
};

}
}
