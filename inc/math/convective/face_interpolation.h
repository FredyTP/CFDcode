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
#include <math/interpolation.h>

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
            virtual void interpolateBCGradient(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value){}
        };

        class UDS : public FaceInterpolation
        {
        public:
            UDS(){}
            virtual void interpolate(SystemSubmatrix* submatrix,FaceVariable facevar,const field::Fields* field)
            {             
                const mesh::Cell* cell=facevar.cell;
                const mesh::Face* face=facevar.face;
                vector2d normal = face->getNormal(cell);
                vector2d vel = field->velocityField(face);
                double u_v = normal.dot(vel);
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
            virtual void interpolateBCGradient(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
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
                    submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef));
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
                submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef/2.0));
                submatrix->addCellVar(CellVariable(cell->index(), face->getOtherCell(cell)->index(), facevar.coef/2.0));
                
            }
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
            {
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
               
                double coef = value * facevar.coef;
                submatrix->addConstant(SystemConstant(cell->index(), -coef));
              
            }
        };

        class PowerLaw : public FaceInterpolation
        {
        public:
            PowerLaw() {}
            virtual void interpolate(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field)
            {
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
                const mesh::Cell* othercell = face->getOtherCell(cell);
                vector2d normal = face->getNormal(cell);
                vector2d vel = field->velocityField(face);
                vector2d r = othercell->getCentroid() - cell->getCentroid();
                vector2d rf = face->getCentroid() - cell->getCentroid();
                double x = rf.norm();
                double L = r.norm();
                double peclet = field->scalarField(cell).density * vel.dot(r) * L / cell->material()->conductivity(field->scalarField(cell));

                double cellcoef=0;
                double othercellcoef=0;
                //math::FaceInterpolation::PowerLaw(peclet, x / L, cellcoef, othercellcoef);
                submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef * cellcoef));
                submatrix->addCellVar(CellVariable(cell->index(), face->getOtherCell(cell)->index(), facevar.coef * othercellcoef));


            }
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
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
            virtual void interpolateBCGradient(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
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
                    submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef));
                }
            }
        };

        class SecondOrderUpWind : public FaceInterpolation
        {
        public:
            SecondOrderUpWind() {}
            virtual void interpolate(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field)
            {
                const mesh::Cell* cell = facevar.cell;
                const mesh::Face* face = facevar.face;
                const mesh::Cell* othercell = face->getOtherCell(cell);
                vector2d normal = face->getNormal(cell);
                vector2d vel = field->velocityField(face);
                double vel_dot_normal = vel.dot(normal);
                vector2d r = othercell->getCentroid() - cell->getCentroid();
                vector2d rf = face->getCentroid() - cell->getCentroid();
                double x = rf.norm();
                double L = r.norm();
                std::vector<CellValue<double>> cellvalues;
                math::FaceInterpolation::SecondOrderUpwing(cell, face, vel_dot_normal, cellvalues);
                for (auto cellval : cellvalues)
                {
                    submatrix->addCellVar(CellVariable(cell->index(), cellval.cell->index(), facevar.coef * cellval.coef));
                }

               


            }
            virtual void interpolateBCValue(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
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
            virtual void interpolateBCGradient(SystemSubmatrix* submatrix, FaceVariable facevar, const field::Fields* field, double value)
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
                    submatrix->addCellVar(CellVariable(cell->index(), cell->index(), facevar.coef));
                }
            }
        };
    }
}
