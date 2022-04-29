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
#include <field/fields.h>

#include <system/problem.h>

#include <solver/matrix_builder.h>
#include <solver/stationary_solver.h>
#include <solver/temporal_solver.h>
#include <solver/stop/stopping_criteria.h>

#include <post/contour.h>
#include <post/time_solution_saver.h>

int main()
{

    sys::Problem problem;
    int n_mesh = 6; //1-6
    int n_cell = pow(4,n_mesh);

    problem.loadProjectMesh(n_cell);

    problem.addConstantMaterial(1.255, 1e-5, 10, 1200);
    problem.assignMaterial();

    //Add Boundary Conditions
    problem.addConstTempBoundary(600);    //BOTTOM
    problem.addConstFluxBoundary(0);      //TOP
    problem.addConstTempBoundary(100);    //LEFT
    problem.addConstTempBoundary(100);    //RIGHT

    problem.loadProjectFaceSelection(1, n_cell); //BOTTOM
    problem.loadProjectFaceSelection(2, n_cell); //TOP
    problem.loadProjectFaceSelection(3, n_cell); //LEFT
    problem.loadProjectFaceSelection(4, n_cell); //RIGHT

    problem.assignBoundaryCondition(0, 0);
    problem.assignBoundaryCondition(1, 1);
    problem.assignBoundaryCondition(2, 2);
    problem.assignBoundaryCondition(3, 3);
   

    //Initialize Fields
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
    std::unique_ptr<term::GradientFlux> orthogonalCorrected = std::make_unique<term::OrthogonalCorrectedGradient>();

    problem.initEquations();
    problem.configConvectiveTerm(secondOrderUpwind.get());
    problem.configDiffusiveTerm(centralDiffGrad.get());

    problem.buildProblem();

    solver::MatrixBuilder matrix;
    solver::TemporalSolver solver(&matrix);
    solver::StationarySolver sSolver(&matrix);

    //STATIONARY SOLVER
    sSolver.solve(&problem);
    post::Contour contour;
    contour.setProblem(&problem);
    contour.setSolution(sSolver.solution());
    contour.saveContourFile("SOUP", true);


    // TEMPORAL SOLVER
    timestep::FixedTimeStep timeStep(0.00001);
    solver::stop::StopAtStep stoppingCriteria(100);

    solver::FixedStepActivity printTimeStep(0);
    solver::FixedStepActivity* pPrint = &printTimeStep;
    printTimeStep.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field)
        {
            std::cout <<"Step number: " << pPrint->step()<<", actual time: "<<time << std::endl;
        });
    solver.addActivity(&printTimeStep);


    post::TimeSolutionSaver solutionSaver;
    solutionSaver.initFile("newsol", 0.05);
    solver::FixedStepActivity saveContour(10);
    solutionSaver.saveSnapshot(problem.fields(), 0.0);
    saveContour.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field) {
        solutionSaver.saveSnapshot(field, time);
        });
    solver.addActivity(&saveContour);
    //SOLVER
    solver.solve(&problem, &timeStep, &stoppingCriteria);
    solutionSaver.save_contour(1080, 720);
    
}
