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

#include <solver/matrix/matrix_builder.h>
#include <solver/matrix/paralel_matrix_builder.h>
#include <solver/stationary_solver.h>
#include <solver/temporal_solver.h>
#include <solver/stop/stopping_criteria.h>

#include <post/contour.h>
#include <post/time_solution_saver.h>
#include <post/sampled_line.h>

#include <tool/chronometer.h>

int main()
{

    sys::Problem problem;
    int n_mesh = 5; //1-6
    int n_cell = pow(4,n_mesh);
    tool::Chronometer timer;
    timer.tic();
    //problem.loadProjectMesh(n_cell);
    
    double w = 0.005;
    double h = 0.1;


    
    problem.createRectMesh(w, h, 84, 1668);
    //problem.createRectMesh(w, h, 10, 100);
    
    problem.addConstantMaterial(1.255, 1e-5, 0.025, 1004.5);
    problem.assignMaterial();

    problem.addConstTempBoundary(300);    //BOTTOM
    problem.addConstTempBoundary(510);      //TOP
    problem.addConstTempBoundary(600);    //LEFT
    problem.addConstTempBoundary(800);   //RIGHT

    std::unique_ptr<mesh::MeshSelection<mesh::Face>> bot_bc = std::make_unique<mesh::MeshSelection<mesh::Face>>();
    bot_bc->selectFromMesh(problem.mesh(), 
        [](mesh::Face* face) {
            if (face->position().y() < 1e-7 && face->isBoundary())
            {
                return true;
            }
            else
            {
                return false;
            }
        });
    std::unique_ptr<mesh::MeshSelection<mesh::Face>> top_bc = std::make_unique<mesh::MeshSelection<mesh::Face>>();
    top_bc->selectFromMesh(problem.mesh(),
        [h](mesh::Face* face) {
            if (face->position().y() >h- 1e-7 && face->isBoundary())
            {
                return true;
            }
            else
            {
                return false;
            }
        });
    std::unique_ptr<mesh::MeshSelection<mesh::Face>> left_bc = std::make_unique<mesh::MeshSelection<mesh::Face>>();
    left_bc->selectFromMesh(problem.mesh(),
        [](mesh::Face* face) {
            if (face->position().x() < 1e-7 && face->isBoundary())
            {
                return true;
            }
            else
            {
                return false;
            }
        });
    std::unique_ptr<mesh::MeshSelection<mesh::Face>> right_bc = std::make_unique<mesh::MeshSelection<mesh::Face>>();
    right_bc->selectFromMesh(problem.mesh(),
        [w](mesh::Face* face) {
            if (face->position().x() >w- 1e-7 && face->isBoundary())
            {
                return true;
            }
            else
            {
                return false;
            }
        });

    problem.addFaceSelection(bot_bc);
    problem.addFaceSelection(top_bc);
    problem.addFaceSelection(left_bc);
    problem.addFaceSelection(right_bc);

    //problem.loadProjectFaceSelection(1, n_cell); //BOTTOM
    //problem.loadProjectFaceSelection(2, n_cell); //TOP
    //problem.loadProjectFaceSelection(3, n_cell); //LEFT
    //problem.loadProjectFaceSelection(4, n_cell); //RIGHT
    

    problem.assignBoundaryCondition(0, 0);
    problem.assignBoundaryCondition(1, 1);
    problem.assignBoundaryCondition(2, 2);
    problem.assignBoundaryCondition(3, 3);
   
    vector2d initialVelocity(0, 0.1);
    field::ScalarStateVector initialScalars;
    initialScalars.pressure = 101325; //Pa
    initialScalars.temperature = 500; //K
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
    timer.tac(true, "building mesh");

    //MATRIX BUILDERS
    solver::ParalelMatrixBuilder paralelBuilder(4);
    solver::MatrixBuilder serialBuilder;
    //STATIC SOLVER
    
    solver::StationarySolver sSolver(&serialBuilder);
    //STATIC SOLVER
    timer.tic();
    sSolver.solve(&problem);
    timer.tac(true, "Solving problem");

    /*post::Contour contour;
    contour.setProblem(&problem);
    timer.tic();
    contour.saveContourFile("TESTFER" + std::to_string(4),&sSolver.solution(), true);
    timer.tac(true, "Saving Results");*/
    
    timer.tic();
    post::SampledLine plot_line(vector2d(w / 2, 0), vector2d(w / 2, h), 1668);
    plot_line.sampleSolution(&problem, sSolver.solution());
    plot_line.saveResults("center_line_temp1668");
    timer.tac(true, "SAVING LINE");
    return 0;
    
    //TEMPORAL SOLVER
    timestep::FixedTimeStep timeStep(0.1);
    solver::stop::StopAtStep stoppingCriteria(50);
    solver::TemporalSolver solver(&serialBuilder);
    solver::FixedStepActivity printTimeStep(1);
    solver::FixedStepActivity* pPrint = &printTimeStep;
    printTimeStep.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field)
        {
            std::cout <<"Step number: " << pPrint->step()<<", actual time: "<<time << std::endl;
        });
    solver.addActivity(&printTimeStep);


    //CONTOUR SAVER CREATION
    post::Contour solutionSaver;
    std::string file_name = "temperature_100";
    solutionSaver.setProblem(&problem);
    solutionSaver.initfile(file_name);
    int currentIteration=0;
    double spacingFactor = 0.03;
    solver::FixedTimeActivity saveContour(0.5);
    solutionSaver.saveSnapshot(problem.fields()->scalarField(field::temperature));
    saveContour.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field) {
        currentIteration++;
        std::cout << "SAVING SOLUTION..." << std::endl;
        double x_offset =  problem->mesh()->boundingBox().size.x() * (1 + spacingFactor) * currentIteration;
        solutionSaver.saveSnapshot(field->scalarField(field::temperature), vector2d(x_offset, 0));
        });
    solver.addActivity(&saveContour);

    timer.tic();
    //SOLVER
    solver.solve(&problem, &timeStep, &stoppingCriteria);
    timer.tac(true, "SIMULATION");
    //solutionSaver.save_contour(10, 1080, 720);

    solutionSaver.initfile("temperature_100_at0.05s");
    solutionSaver.saveSnapshot(solver.solution()->scalarField(field::temperature));
    solutionSaver.save_contour(10, 1080, 720);

}
