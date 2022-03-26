/*****************************************************************//**
 * \file   face_interpolation.h
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
        class FaceInterpolation
        {
        public:
            FaceInterpolation(){}
            virtual void interpolate(SystemSubmatrix* submatrix, FaceVariable facevar,const field::Fields* field) = 0;
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar,const field::Fields* field, double value) = 0;
        };

        class UDS : public FaceInterpolation
        {
        public:
            UDS(){}
            virtual void interpolate(SystemSubmatrix* submatrix,FaceVariable facevar,const field::Fields* field)
            {             
                const mesh::Cell* cell=facevar.cell;
                const mesh::Face* face=facevar.face;
                double u_v = face->getNormal(cell).dot(field->velocityField(face));
                if (u_v > 0)
                {
                    submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef));
                }
                else
                {
                    submatrix->addCellVar(CellVariable(cell->index(), face->getOtherCell(cell)->index(), facevar.coef));
                }
            }
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar,const field::Fields* field, double value)
            {
                
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
                double u_v = face->getNormal(cell).dot(field->velocityField(face));
                
                if (u_v > 0)
                {
                    submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef));
                }
                else
                {
                    double coef = value * facevar.coef;
                    submatrix->addConstant(SystemConstant(cell->index(), -coef));
                }
            }
        };

        class CDS : public FaceInterpolation
        {
        public:
            CDS() {}
            virtual void interpolate(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field)
            {
                
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
                double u_v = face->getNormal(cell).dot(field->velocityField(face));
                submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef/2));
                submatrix->addCellVar(CellVariable(cell->index(), face->getOtherCell(cell)->index(), facevar.coef/2));
                
            }
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
            {
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
                

                double coef = value * facevar.coef;
                submatrix->addConstant(SystemConstant(cell->index(), -coef));
              
            }
        };
    }
}
