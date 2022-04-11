/*****************************************************************//**
 * \file   stationary_solver.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

#include <solver/matrix_builder.h>
#include <Eigen/SparseLU>

namespace solver
{
    class StationarySolver
    {
    public:
        StationarySolver(MatrixBuilder* _builder_) : _builder(_builder_) {}

        void solve(const mesh::Mesh* pMesh,
        const std::vector<std::unique_ptr<bc::BoundaryCondition>>& _bConditions,
        math::diffusive::DiffusiveTerm* pDiffusive, math::convective::ConvectiveTerm* pConvective,
        const field::Fields* pField)
        {
            _builder->buildSystem(pMesh, _bConditions, pDiffusive, pConvective, pField);

            Eigen::SparseLU<Eigen::SparseMatrix<double>> chol(_builder->getMatrix());
            _solution = chol.solve(_builder->getVector());

            initfile("static_solution.txt");
            save("static_solution.txt", _solution, pMesh);
            save_contour("static_solution.txt", "static_contour.jpg", 1920, 1080);
                
        }
        Eigen::VectorXd solution() { return _solution; }
        void initfile(const std::string& filename)
        {
            std::ofstream file;
            file.open(filename, std::ios::trunc);
            if (file.is_open())
            {
                file << "X" << "," << "Y" << "," << "Value" << "," << "VX1" << "," << "VY1" << "," << "VX2" << "," << "VY2" << "," << "VX3" << "," << "VY3" << std::endl;
                file.close();
            }
        }
        void save(const std::string& filename, const Eigen::VectorXd& _solution, const mesh::Mesh* pMesh)
        {
            std::ofstream file;
            file.open(filename, std::ios::app);
            if (file.is_open())
            {
                for (int i = 0; i < _solution.size(); i++)
                {
                    auto cell = pMesh->cells()->at(i).get();
                    auto n0 = cell->nodes().at(0);
                    auto n1 = cell->nodes().at(1);
                    auto n2 = cell->nodes().at(2);

                    vector2d pos = pMesh->cells()->at(i)->getCentroid();

                    file << pos.x() << "," << pos.y() << "," << _solution(i)
                        << "," << n0->pos().x() << "," << n1->pos().x() << "," << n2->pos().x() 
                        << "," << n0->pos().y() << "," << n1->pos().y() << "," << n2->pos().y() 
                        << std::endl;
                }
                file.close();
            }
        }
        void save_contour(const std::string& filename, const std::string& contour_filename, size_t Rx, size_t Ry)
        {
            std::cout << "Saving: " << filename << " to: " << contour_filename << " ..." << std::endl;
            std::string command = "matlab  -batch \"plot_result('" + filename + "','" + contour_filename + "'," + std::to_string(Rx) + "," + std::to_string(Ry) + ")\"";
            system(command.c_str());
            std::cout << "saved" << std::endl;
            std::string openimg = "\"" + contour_filename + "\"";
            system(openimg.c_str());
        }
    private:
        Eigen::VectorXd _solution;
        MatrixBuilder* _builder;
    };
}
