#include "environment.h"
#include <mesh/reader/luismi_format_mesh_reader.h>
#include <material/material_factory.h>
#include <material/properties/constant_properties.h>
#include <Eigen/Sparse>
#include <boundary/constant_temperature.h>
#include <solver/matrix_builder.h>
#include <chrono>
#include <solver/stationary_solver.h>
#include <solver/temporal_solver.h>
#include <boundary/constant_gradient.h>
#include <mesh/manipulation/mesh_selection.h>

core::Environment::Environment()
{
}

void core::Environment::loadMesh()
{
    using namespace mesh;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
 
    LuisMiformatMeshReader meshReader("D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//nodes_4096.dat",
    "D://Alfredo//Academic//MASTER AERO//B//CFD//ProyectoCFD//CodigoCFD//mesh//cells_4096.dat");
    _mesh = std::make_unique<Mesh>();
    _mesh->loadMesh(&meshReader);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time loading mesh = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
}

void core::Environment::computeMesh()
{
    
}

void core::Environment::buildMaterials()
{
    using namespace material;
    MaterialFactory matFactory;
    matFactory.setDensityModel<prop::ConstantDensity>(1.225);
    matFactory.setViscosityModel<prop::ConstantViscosity>(1e-5);
    matFactory.setConductivityModel<prop::ConstantConductivity>(10);
    matFactory.setSpecificHeatModel<prop::ConstantSpecificHeat>(1040);

    _fluidMaterial = matFactory.extractMaterial();
    for (size_t cellid = 0; cellid < _mesh->cells()->size(); cellid++)
    {
        _mesh->cells()->at(cellid)->setMaterial(_fluidMaterial.get());
    }
}

void core::Environment::createBoundary()
{
    mesh::reader::FaceReader faceReader1("mesh//bc_1_4096.dat");
    mesh::reader::FaceReader faceReader2("mesh//bc_2_4096.dat");
    mesh::reader::FaceReader faceReader3("mesh//bc_3_4096.dat");
    mesh::reader::FaceReader faceReader4("mesh//bc_4_4096.dat");

    mesh::MeshSelection<mesh::Face> sel1;
    mesh::MeshSelection<mesh::Face> sel2;
    mesh::MeshSelection<mesh::Face> sel3;
    mesh::MeshSelection<mesh::Face> sel4;

    sel1.selectWithReader(_mesh.get(), &faceReader1);
    sel2.selectWithReader(_mesh.get(), &faceReader2);
    sel3.selectWithReader(_mesh.get(), &faceReader3);
    sel4.selectWithReader(_mesh.get(), &faceReader4);

    std::unique_ptr<bc::BoundaryCondition> bc1 = std::make_unique<bc::ConstantTemperature>(300);
    std::unique_ptr<bc::BoundaryCondition> bc2 = std::make_unique<bc::ConstantFlux>(0);
    std::unique_ptr<bc::BoundaryCondition> bc3 = std::make_unique<bc::ConstantTemperature>(300);
    std::unique_ptr<bc::BoundaryCondition> bc4 = std::make_unique<bc::ConstantTemperature>(300);

    bc1->setBoundary(sel1);
    bc2->setBoundary(sel2);
    bc3->setBoundary(sel3);
    bc4->setBoundary(sel4);

    _boundaryConditions.push_back(std::move(bc1));
    _boundaryConditions.push_back(std::move(bc2));
    _boundaryConditions.push_back(std::move(bc3));
    _boundaryConditions.push_back(std::move(bc4));
}

void core::Environment::initializeFields()
{
    using namespace field;
    _fields = std::make_unique<Fields>(_mesh.get());
    auto& densityField = _fields->rawDensity();
    auto& tempField = _fields->rawTemperature();
    auto& pressField = _fields->rawPressure();
    auto& velField = _fields->rawVelocity();

    mesh::MeshSelection<mesh::Cell> selection;
    selection.selectFromMesh(_mesh.get(), [](mesh::Cell* cell)
        {
            vector2d pos = cell->position();
            vector2d center(0.01/4,0.1/2);
            double x = pos.x();
            double y = pos.y();
            double px = center.x();
            double py = center.y();
            double a = 0.005/3;
            double b = 0.025;
            if (std::pow((x - px) / a, 2) + std::pow((y - py) / b, 2) <= 1)
            {
                return true;
            }
            else
            {
                return false;
            }});

    for (size_t i = 0; i < densityField.size(); i++)
    {
        
        densityField[i] = 1.225; //_fluidMaterial->density((*escalarField)[i]);
        pressField[i] = 101325;
        tempField[i] = 298.15;
    }
    for (size_t i = 0; i < velField.size(); i++)
    {        
        velField[i].x() = 0;
        velField[i].y() = 10;
    }

    selection.for_each([&tempField](mesh::Cell* cell) {tempField[cell->index()] = 600.0;});
    
}

void core::Environment::solve()
{
    using namespace math::convective;
    using namespace math::diffusive;


    //Incluir esto en una lista de metodos de discretización para que se pueda elegir via terminal...
    //Face interpolation methods
    std::unique_ptr<FaceInterpolation> uds = std::make_unique<UDS>();
    std::unique_ptr<FaceInterpolation> cds = std::make_unique<CDS>();
    std::unique_ptr<FaceInterpolation> powerLaw = std::make_unique<PowerLaw>();
    std::unique_ptr<FaceInterpolation> secondOrderUpwind = std::make_unique<SecondOrderUpWind>();

    //Surface Integral methods
    std::unique_ptr<SurfaceIntegral> midpoint = std::make_unique<MidPoint>();

   
    //Gradient Flux methods
    std::unique_ptr <GradientFlux> centralDiffGrad=std::make_unique<CentralDifferenceGradient>();

    //EQUATION TERMS
    std::unique_ptr<ConvectiveTerm> convectiveTerm = std::make_unique<ConvectiveTerm>(midpoint.get(), secondOrderUpwind.get());
    std::unique_ptr<DiffusiveTerm> diffusiveTerm = std::make_unique<DiffusiveTerm>(centralDiffGrad.get());
    


    for (int i = 0; i < _boundaryConditions.size(); i++)
    {
        _boundaryConditions[i]->setConvetiveTerm(convectiveTerm.get());
        _boundaryConditions[i]->setDiffusiveTerm(diffusiveTerm.get());
    }
    solver::MatrixBuilder matrix;
    solver::TemporalSolver solver(&matrix);
    solver::StationarySolver sSolver(&matrix);

   
    //
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int i = 0;i < 1; i++)
    {
       //sSolver.solve(_mesh.get(), _boundaryConditions, diffusiveTerm.get(), convectiveTerm.get(), _fields.get());
        solver.solve(_mesh.get(), _boundaryConditions, diffusiveTerm.get(), convectiveTerm.get(), _fields.get());
        //matrix.solve();
        //matrix.save("Result.txt");
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    
    //system("plotter.exe");


}
