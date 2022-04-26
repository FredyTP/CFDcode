/*****************************************************************//**
 * \file   convective_term.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <memory>

 //CFD

#include <equation/convective/face_interpolation.h>
#include <equation/equation_term.h>
#include <math/var/system_submatrix.h>
#include <field/fields.h>

namespace term
{

    //Generalize this concept in equation terms
    class ConvectiveTerm : public FaceEquationTerm
    {
    public:
        
        ConvectiveTerm(FaceInterpolation* faceInterpolation, field::scalarType scalarField) : FaceEquationTerm(scalarField) {
            faceInterpolationScheme = faceInterpolation;
            if (scalarField == field::temperature)
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetTemperatureConvectionCoeficient;
            }
            else if (scalarField == field::velocity_x)
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetVelocityXConvectionCoeficient;
            }
            else if (scalarField == field::velocity_y)
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetVelocityYConvectionCoeficient;
            }
            else if (scalarField == field::continuity_x)
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetContinuityXConvectionCoeficient;
            }
            else if (scalarField == field::continuity_y)
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetContinuityYConvectionCoeficient;
            }
            else
            {
                _GetConvectionCoeficient = ConvectiveTerm::GetNullCoeficient;
            }

        }
        
        void calculateBothCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields) const
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double area = face->area();
            vector2d normal = face->getNormal(cell1);

            double u1 = fields->scalarField(field::velocity_x, cell1);
            double u2 = fields->scalarField(field::velocity_x, cell2);
            double v1 = fields->scalarField(field::velocity_y, cell1);
            double v2 = fields->scalarField(field::velocity_y, cell2);

            vector2d velocity((u1 + u2) / 2, (v1 + v2) / 2);
            double coef = _GetConvectionCoeficient(face, fields) * face->area() * normal.dot(velocity);

            math::FaceValue<double> facevar(face, coef);
            std::vector<math::CellValue<double>> cellvalues;
            faceInterpolationScheme->interpolateFace(cellvalues,facevar, fields);
            submatrix->addFaceValues(face, cellvalues);
 
        }
        void calculateOneCell(math::SystemSubmatrix* submatrix, const mesh::Face* face, const field::Fields* fields, bool isBoundaryCell = false) const
        {

            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double area = face->area();
            vector2d normal = face->getNormal(cell1);

            double u1 = fields->scalarField(field::velocity_x, cell1);
            double u2 = fields->scalarField(field::velocity_x, cell2);
            double v1 = fields->scalarField(field::velocity_y, cell1);
            double v2 = fields->scalarField(field::velocity_y, cell2);

            vector2d velocity((u1 + u2) / 2, (v1 + v2) / 2);
            double coef = _GetConvectionCoeficient(face, fields) * face->area() * normal.dot(velocity);

            math::FaceValue<double> facevar(face, coef);
            std::vector<math::CellValue<double>> cellvalues;
            faceInterpolationScheme->interpolateFace(cellvalues, facevar, fields);
            submatrix->addCellValues(isBoundaryCell ? face->cell2() : face->cell1(), cellvalues, isBoundaryCell);
        }
        
        FaceInterpolation* getFaceInterpolation() { return faceInterpolationScheme; }

        void setFaceInterpolation(FaceInterpolation* faceInterpolation)
        {
            faceInterpolationScheme = faceInterpolation;
        }

        static double GetNullCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            return 0;
        }
        static double GetTemperatureConvectionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double density1 = fields->scalarField(field::density, cell1);
            double density2 = fields->scalarField(field::density, cell2);

            double cv1 = cell1->material()->specificHeat(cell1, fields);
            double cv2 = cell2->material()->specificHeat(cell2, fields);


            return (density1 + density2) / 2 * (cv1 + cv2) / 2;
        }
        static double GetVelocityXConvectionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double density1 = fields->scalarField(field::density, cell1);
            double density2 = fields->scalarField(field::density, cell2);

            double u1 = fields->scalarField(field::velocity_x, cell1);
            double u2 = fields->scalarField(field::velocity_x, cell2);


            return (density1 + density2) / 2 * (u1 + u2) / 2;
        } 
        static double GetVelocityYConvectionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double density1 = fields->scalarField(field::density, cell1);
            double density2 = fields->scalarField(field::density, cell2);

            double v1 = fields->scalarField(field::velocity_y, cell1);
            double v2 = fields->scalarField(field::velocity_y, cell2);
            return (density1 + density2) / 2 * (v1 + v2) / 2;
        }
        static double GetContinuityXConvectionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double density1 = fields->scalarField(field::density, cell1);
            double density2 = fields->scalarField(field::density, cell2);

            double u1 = fields->scalarField(field::velocity_x, cell1);
            double u2 = fields->scalarField(field::velocity_x, cell2);
            double v1 = fields->scalarField(field::velocity_y, cell1);
            double v2 = fields->scalarField(field::velocity_y, cell2);

            vector2d velocity((u1 + u2) / 2, (v1 + v2) / 2);

            return (density1 + density2) * face->normal1().x() / (2 * velocity.dot(face->normal1()));
        }
        static double GetContinuityYConvectionCoeficient(const mesh::Face* face, const field::Fields* fields)
        {
            auto cell1 = face->cell1();
            auto cell2 = face->cell2();
            double density1 = fields->scalarField(field::density, cell1);
            double density2 = fields->scalarField(field::density, cell2);

            double u1 = fields->scalarField(field::velocity_x, cell1);
            double u2 = fields->scalarField(field::velocity_x, cell2);
            double v1 = fields->scalarField(field::velocity_y, cell1);
            double v2 = fields->scalarField(field::velocity_y, cell2);

            vector2d velocity((u1 + u2) / 2, (v1 + v2) / 2);

            return (density1 + density2) * face->normal1().y() / (2 * velocity.dot(face->normal1()));
        }

    private:
        std::function<double(const mesh::Face*, const field::Fields*)> _GetConvectionCoeficient;
        FaceInterpolation* faceInterpolationScheme;
    };



}
