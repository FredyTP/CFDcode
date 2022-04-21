// CodigoCFD.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//


#include <Eigen/Eigen>

#include <mesh/mesh.h>
#include <mesh/reader/luismi_format_mesh_reader.h>


#include <material/material.h>
#include <material/properties/constant_properties.h>
#include <material/properties/ideal_gas.h>
#include <material/material_factory.h>

#include <field/state_vector.h>
#include <field/field.h>

#include <system/problem.h>

#include <solver/matrix_builder.h>
#include <solver/stationary_solver.h>
#include <solver/temporal_solver.h>

int main()
{

    sys::Problem problem;
    int n_cell = 4096;

    problem.loadProjectMesh(n_cell);

    problem.addConstantMaterial(1.255, 1e-5, 10, 1200);
    problem.assignMaterial();

    problem.addConstTempBoundary(300);
    problem.addConstTempBoundary(300);
    problem.addConstTempBoundary(600);
    problem.addConstTempBoundary(600);

    problem.loadProjectFaceSelection(1, n_cell);
    problem.loadProjectFaceSelection(2, n_cell);
    problem.loadProjectFaceSelection(3, n_cell);
    problem.loadProjectFaceSelection(4, n_cell);

    problem.assignBoundaryCondition(0, 0);
    problem.assignBoundaryCondition(1, 1);
    problem.assignBoundaryCondition(2, 2);
    problem.assignBoundaryCondition(3, 3);
   
    vector2d initialVelocity(0, 10);
    field::ScalarStateVector initialScalars;
    initialScalars.pressure = 101325; //Pa
    initialScalars.temperature = 300; //K
    problem.initializeFields(initialVelocity, initialScalars);


    //DISCRETIZATION METHODS
    //Face Intertpolation
    std::unique_ptr<term::FaceInterpolation> uds = std::make_unique<term::UDS>();
    std::unique_ptr<term::FaceInterpolation> cds = std::make_unique<term::CDS>();
    std::unique_ptr<term::FaceInterpolation> powerLaw = std::make_unique<term::PowerLaw>();
    std::unique_ptr<term::FaceInterpolation> secondOrderUpwind = std::make_unique<term::SecondOrderUpWind>();
    //Gradient Flux
    std::unique_ptr<term::GradientFlux> centralDiffGrad = std::make_unique<term::CentralDifferenceGradient>();

    problem.initEquations();
    problem.configConvectiveTerm(secondOrderUpwind.get());
    problem.configDiffusiveTerm(centralDiffGrad.get());

    problem.buildProblem();

    solver::MatrixBuilder matrix;
    solver::TemporalSolver solver(&matrix);
    solver::StationarySolver sSolver(&matrix);

    solver.solve(&problem);

    /*simulation.loadMesh();
    simulation.buildMaterials();
    simulation.createBoundary();
    simulation.initializeFields();
    simulation.solve();*/
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
