/*****************************************************************//**
 * \file   matrix_builder.cpp
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <solver/matrix_builder.h>
#include <field/field.h>
#include <fstream>

namespace solver
{
    MatrixBuilder::MatrixBuilder()
    {
    }
    void MatrixBuilder::buildSystem(const mesh::Mesh* pMesh,const math::GradientScheme* gradientScheme,const bc::BoundaryCondition* _bCondition,field::Field* _field)
    {
        auto cells = pMesh->cells().get();
        independent.resize(cells->size());
        independent.setZero();
        auto faces = pMesh->faces().get();
        for (size_t cellid = 0; cellid < cells->size(); cellid++)
        {
            auto cell = cells->at(cellid).get();
            
            for (auto face : cell->faces())
            {
                if (face->isBoundary())
                {
                    //std::vector< Eigen::Triplet<double>> grad_coef;// =gradientScheme->computeFaceCoefs(cell, face);
                    //grad_coef.push_back(Eigen::Triplet<double>());
                    /*double K = 1000;
                    double flux = face->getUnitFlux(cell) * K;
                   
                        
                    independent(cellid) += 1 * flux*300;
                        
                    _diffusive.push_back(Eigen::Triplet<double>(cell->index(), cell->index(), 1 * flux));
                      */  
                    
                }
                else
                {
                    auto grad_coef = gradientScheme->computeFaceCoefs(cell, face);
                    double K = 1000;
                    double flux = face->getUnitFlux(cell) * K;
                    
                    for (auto triplet : grad_coef)
                    {
                        _diffusive.push_back(Eigen::Triplet<double>(triplet.row(), triplet.col(), triplet.value() * flux));
                        
                    }

                    //REMOVE THIS -> CONVECTIVE UPWIND
                    vector2d vel = _field->velocityField()->at(face->index());
                    double nu = face->getNormal(cell).dot(vel);
                    double cv = 1024;
                    double rho = 1.225;
                    double area = face->area();
                    double convection = cv * rho * nu * area;
                    if (nu > 0)
                    {
                        //THIS IS CONVECTIVE!!!
                        _diffusive.push_back(Eigen::Triplet<double>(cell->index(), cell->index(), convection));
                    }
                    else
                    {
                        // THIS IS CONVECTIVE!!!
                        _diffusive.push_back(Eigen::Triplet<double>(cell->index(), face->getOtherCell(cell)->index(), convection));
                    }

                    //ADD SOURCE TERMS?
                }
                
            }
        }
        auto bcond = _bCondition->getBCCoefs(pMesh, _field);
        for (auto bctriplet : bcond.first)
        {
            _diffusive.push_back(bctriplet);
        }
        independent += bcond.second;
        //independent(3) = independent(2);
        Eigen::SparseMatrix<double> sparse(cells->size(), cells->size());
        sparse.setFromTriplets(_diffusive.begin(), _diffusive.end(), [](double phi1, double phi2) {
            return phi1 + phi2;
    });
        Eigen::SimplicialCholesky < Eigen::SparseMatrix<double> > chol(sparse);
        // realiza una factorización de Cholesky de A

        Eigen::VectorXd x = chol.solve(independent);
        //std::cout << sparse << std::endl;
        
        //std::cout << independent << std::endl;
        std::cout << x << std::endl;

        //Export solution
        // X[m] Y[m] value[K]
        std::ofstream file;
        file.open("Result.txt", std::ios::trunc);
        if (file.is_open())
        {
            file << "X" << "," << "Y" << "," << "Value" << std::endl;
            for (int i = 0; i < x.size(); i++)
            {

                vector2d pos = cells->at(i)->getCentroid();
                file << pos.x() << "," << pos.y() << "," << x(i) << std::endl;
            }
            file.close();
        }
    }
    
}
