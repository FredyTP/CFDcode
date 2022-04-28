#pragma once
/*****************************************************************//**
 * \file   stationary_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <solver/matrix_builder.h>
#include <Eigen/SparseLU>
#include <math/interpolation/gradient_interpolation.h>

namespace solver
{
    class StationarySolver
    {
    public:
        StationarySolver(MatrixBuilder* _builder_) : _builder(_builder_) {}

        void solve(sys::Problem* problem)
        {
            while (1)
            {
                Eigen::SparseMatrix<double> matrix;
                Eigen::SparseMatrix<double> Gx;
                Eigen::SparseMatrix<double> Gy;
                Eigen::SparseMatrix<double> Cu; //Continuity Matrix X
                Eigen::SparseMatrix<double> Cv; //Continuity Matrix Y

                Eigen::SparseMatrix<double> Du; //Diagonal Velocity Matrix X
                Eigen::SparseMatrix<double> Dv; //Diagonal Velocity Matrix Y
                Eigen::SparseMatrix<double> M1; //Velocity Matrix X
                Eigen::SparseMatrix<double> M2; //Velocity Matrix Y
                Eigen::VectorXd vector;
                this->GxMatrix(Gx, problem);
                this->GyMatrix(Gy, problem);

                //VELOCITY X
                _builder->buildSystem(problem, field::velocity_x);
                matrix = _builder->getMatrix();
                M1 = matrix;

                Du = matrix.diagonal().cwiseInverse().asDiagonal();
                std::cout << Du << std::endl;

                std::cout << "VELOCITY X" << std::endl;
                std::cout << "------------" << std::endl;
                vector = _builder->getVector() - Gx * problem->fields()->scalarField(field::pressure);
                this->solveEquation(matrix, vector, field::velocity_x);

                std::cout << "VELOCITY Y" << std::endl;
                std::cout << "------------" << std::endl;
                _builder->buildSystem(problem, field::velocity_y);
                M2 = matrix;
                matrix = _builder->getMatrix();
                Dv = matrix.diagonal().cwiseInverse().asDiagonal();

                vector = _builder->getVector() - Gy * problem->fields()->scalarField(field::pressure);
                this->solveEquation(matrix, vector, field::velocity_y);


                _builder->buildSystem(problem, field::continuity_x);
                Cu = _builder->getMatrix();

                _builder->buildSystem(problem, field::continuity_y);
                Cv = _builder->getMatrix();
                std::cout << "CONTINUTY X - Y" << std::endl;
                std::cout << "------------" << std::endl;
                std::cout << Cu << std::endl;
                std::cout << Cv << std::endl;
                std::cout << Cu * _solution[field::velocity_x] + Cv * _solution[field::velocity_y] << std::endl;

                std::cout << "PRESSURE CORRECTION" << std::endl;
                std::cout << "------------" << std::endl;
                matrix = Cu * Du * Gx + Cv * Dv * Gy;
                vector = Cu * _solution[field::velocity_x] + Cv * _solution[field::velocity_y];
             
                for (int i = 0; i < 10; i++)
                {
                    for (int j = 0; j < 10; j++)
                    {
                        if (i >= 4 || j >= 4)
                        {
                            matrix.coeffRef(i, j) = 0;
                            if (i == j)
                            {
                                matrix.coeffRef(i, j) = 1;
                            }
                        }
                    }
                    if (i >= 4)
                    {
                        vector(i) = 0;
                    }
                }
                
                //Eigen::VectorXd H1; 
                //Eigen::VectorXd H2;
                // H1 = M1 * _solution[field::velocity_x] - Du * _solution[field::velocity_x];
                // H2 = M2 * _solution[field::velocity_y] - Dv * _solution[field::velocity_y];

                //matrix = Cu * Du * Gx + Cv * Dv * Gy;
                //vector = Cu * Du * H1 + Cv * Dv * H2;
                this->solveEquation(matrix, vector, field::pressure_correction);

                double relaxationFactor = 0.1;
                problem->fields()->scalarField(field::velocity_x) = _solution[field::velocity_x];
                problem->fields()->scalarField(field::velocity_y) = _solution[field::velocity_y];

                problem->fields()->scalarField(field::pressure) += relaxationFactor * _solution[field::pressure_correction];
                problem->fields()->scalarField(field::velocity_x) += relaxationFactor * Du * Gx * _solution[field::pressure_correction];
                problem->fields()->scalarField(field::velocity_y) += relaxationFactor * Dv * Gy * _solution[field::pressure_correction];
                Eigen::VectorXd correction = relaxationFactor * Du * Gx * problem->fields()->scalarField(field::pressure_correction);
                
                std::cout << "PRESSURE - VX - VY" << std::endl;
                std::cout << "------------" << std::endl;
                std::cout << problem->fields()->scalarField(field::pressure) << std::endl << std::endl;
                std::cout << problem->fields()->scalarField(field::velocity_x) << std::endl << std::endl;
                std::cout << problem->fields()->scalarField(field::velocity_y) << std::endl << std::endl;
            }

        }
        void solveEquation(Eigen::SparseMatrix<double>& matrix,Eigen::VectorXd& vector, field::scalarType equation)
        {           
            std::cout << "Matrix:" << std::endl;
            std::cout << matrix << std::endl;
            std::cout << "Vector" << std::endl;
            std::cout << vector << std::endl;
            Eigen::SparseLU<Eigen::SparseMatrix<double>> chol(matrix);
            _solution[equation] = chol.solve(vector);
            std::cout << "solution" << std::endl;
            std::cout << _solution[equation] << std::endl;
        }
        Eigen::VectorXd& solution(field::scalarType equation) { return _solution[equation]; }
        
        void GxMatrix(Eigen::SparseMatrix<double>& matrix,sys::Problem* problem)
        {
            size_t numb_equation = problem->mesh()->cells()->size();
            math::SystemSubmatrix submatrix(problem->mesh()->cells()->size());
            double volume;
            for (auto& cell : *problem->mesh()->cells())
            {
                if (cell->faces().size() > 1)
                {
                    volume = cell->volume();
                    std::vector <math::CellValue<vector2d>> gradient;
                    std::vector <math::CellValue<double>> gradientx;
                    math::GradientInterpolation::GreenGaussGradient(cell.get(), gradient);
                    math::GradientInterpolation::DotProduct(gradientx, gradient, vector2d(1, 0));
                    submatrix.addCellValues(cell.get(), gradientx);
                }
                else
                {
                    std::vector <math::CellValue<vector2d>> gradient;
                    std::vector <math::CellValue<double>> gradientx;
                    math::GradientInterpolation::FiniteDifferenceGradient(cell->faces()[0], gradient);
                    math::GradientInterpolation::DotProduct(gradientx, gradient, vector2d(1, 0));
                    submatrix.addCellValues(cell.get(), gradientx);
                }
            }

            auto pressure_triples = submatrix.cellCoeficients();
            Eigen::SparseMatrix<double> pressureMatrix;
            matrix.data().clear();
            matrix.resize(numb_equation, numb_equation);
            matrix.setFromTriplets(pressure_triples.begin(), pressure_triples.end(),
                [](double phi1, double phi2) {
                        return phi1 + phi2;
            });

                //Eigen::VectorXd pressureSource = pressureMatrix * problem->fields()->scalarField(field::pressure);
                //_independent -= pressureSource;
        }
        void GyMatrix(Eigen::SparseMatrix<double>& matrix,sys::Problem* problem)
        {
            size_t numb_equation = problem->mesh()->cells()->size();
            math::SystemSubmatrix submatrix(problem->mesh()->cells()->size());
            double volume;
            for (auto& cell : *problem->mesh()->cells())
            {
                if (cell->faces().size() > 1)
                {
                    volume = cell->volume();
                    std::vector <math::CellValue<vector2d>> gradient;
                    std::vector <math::CellValue<double>> gradientx;
                    math::GradientInterpolation::GreenGaussGradient(cell.get(), gradient);
                    math::GradientInterpolation::DotProduct(gradientx, gradient, vector2d(0, 1));
                    submatrix.addCellValues(cell.get(), gradientx);
                }
                else
                {
                    std::vector <math::CellValue<vector2d>> gradient;
                    std::vector <math::CellValue<double>> gradientx;
                    math::GradientInterpolation::FiniteDifferenceGradient(cell->faces()[0], gradient);
                    math::GradientInterpolation::DotProduct(gradientx, gradient, vector2d(1, 0));
                    submatrix.addCellValues(cell.get(), gradientx);
                }

            }

            auto pressure_triples = submatrix.cellCoeficients();
            Eigen::SparseMatrix<double> pressureMatrix;
            matrix.data().clear();
            matrix.resize(numb_equation, numb_equation);
            matrix.setFromTriplets(pressure_triples.begin(), pressure_triples.end(),
                [](double phi1, double phi2) {
                        return phi1 + phi2;
            });

                //Eigen::VectorXd pressureSource = pressureMatrix * problem->fields()->scalarField(field::pressure);
                //_independent -= pressureSource;
        }
    private:
        std::array<Eigen::VectorX<double>, field::scalar_field_number> _solution;
        MatrixBuilder* _builder;
    };
}
