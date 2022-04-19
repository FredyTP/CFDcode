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
#include <math/interpolation/face_interpolation.h>
#include <math/adimensional.h>

#include <field/field.h>


namespace math
{
    namespace convective
    {
        class FaceInterpolation
        {
        public:
            FaceInterpolation(){}
            virtual void interpolateFace(std::vector<CellValue<double>> &cellvalues, const FaceVariable& facevar,const field::Fields* field) = 0;          
            
        };

        class UDS : public FaceInterpolation
        {
        public:
            UDS(){}
            virtual void interpolateFace(std::vector<CellValue<double>>& cellvalues, const FaceVariable& facevar,const field::Fields* field)
            {             
                
                const mesh::Face* face=facevar.face;
                vector2d normal = face->normal1();
                vector2d vel = field->velocityField(face);
                double u_v = normal.dot(vel);
                
                math::FaceInterpolation::UpwindDifferencingScheme(facevar.face, u_v, cellvalues, facevar.coef);
          
            }           

        };

        class CDS : public FaceInterpolation
        {
        public:
            CDS() {}
            virtual void interpolateFace(std::vector<CellValue<double>>& cellvalues, const FaceVariable& facevar, const field::Fields* field)
            {                            
                const mesh::Face* face = facevar.face;               
                math::FaceInterpolation::CentralDifferencingScheme(face, face->lambda(), cellvalues, facevar.coef);               
            }                       
        };

        class PowerLaw : public FaceInterpolation
        {
        public:
            PowerLaw() {}
            virtual void interpolateFace(std::vector<CellValue<double>>& cellvalues, const FaceVariable& facevar, const field::Fields* field)
            {
                const mesh::Face* face = facevar.face;
                const mesh::Cell* cell = face->cell1();
                const mesh::Cell* othercell = face->cell2();

                //GEOMETRY
                vector2d normal = face->getNormal(cell);
                vector2d vel = field->velocityField(face);
                vector2d cell2cell = othercell->getCentroid() - cell->getCentroid();
                vector2d cell2face = face->getCentroid() - cell->getCentroid();
                double x = cell2face.norm();
                double L = cell2cell.norm();

                //LAMBDA
                double lambda = math::Adimensional::Lambda(x, L);

                //PECLET
                double density = field->scalarField(cell).density;
                double speed = vel.dot(cell2cell);;
                double conductivity = 10;//cell->material()->conductivity(field->scalarField(cell));
                double peclet_number = math::Adimensional::PecletNumber(density, speed, L, conductivity);
             
                math::FaceInterpolation::PowerLaw(facevar.face,peclet_number, lambda, cellvalues, facevar.coef);
            }
            
           
        };

        class SecondOrderUpWind : public FaceInterpolation
        {
        public:
            SecondOrderUpWind() {}
            virtual void interpolateFace(std::vector<CellValue<double>>& cellvalues, const FaceVariable& facevar, const field::Fields* field)
            {
                const mesh::Face* face = facevar.face;
                const mesh::Cell* cell = face->cell1();
                const mesh::Cell* othercell = face->cell2();
                vector2d normal = face->getNormal(cell);
                vector2d vel = field->velocityField(face);
                double vel_dot_normal = vel.dot(normal);
                vector2d r = othercell->getCentroid() - cell->getCentroid();
                vector2d rf = face->getCentroid() - cell->getCentroid();
                double x = rf.norm();
                double L = r.norm();
               
                math::FaceInterpolation::SecondOrderUpwind(cell, face, vel_dot_normal, cellvalues, facevar.coef);        
            }
            
        };
    }
}
