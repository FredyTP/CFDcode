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

#include <solver/propagator/propagator.h>
void solveProjectProblem(bool tri_mesh,int n_mesh ,term::FaceInterpolation* faceInterpolation, term::GradientFlux* gradientFlux,solver::Propagator* propagator, double density,double conductivity,double specificHeat,vector2d velocity,double temperature,bool temporal)
{
    sys::Problem problem;
    int n_cell = pow(4, n_mesh);

    std::cout << "SOLVING PROBLEM FOR: N_CELL = " + std::to_string(n_cell) + " FACE INTERPOLATION = " + faceInterpolation->name() + " & GRADIENT: " + gradientFlux->name() << std::endl;
    tool::Chronometer timer;
    timer.tic();
    std::string mesh_name;
    if (tri_mesh)
    {
        mesh_name = "TRI";

        //IMPORT PROJECT MESH//
        problem.loadProjectMesh(n_cell);

        //_____BOUNDARY CONDITION LOADING____//
        problem.loadProjectFaceSelection(1, n_cell); //BOTTOM
        problem.loadProjectFaceSelection(2, n_cell); //TOP
        problem.loadProjectFaceSelection(3, n_cell); //LEFT
        problem.loadProjectFaceSelection(4, n_cell); //RIGHT
    }
    else
    {
        mesh_name = "QUAD";
        double w = 0.005;
        double h = 0.1;

        int n_w = ceil(sqrt(pow(4, n_mesh) / 10));
        int n_h = floor(10 * sqrt(pow(4, n_mesh) / 10));
        problem.createRectMesh(w, h, n_w, n_h);

        //CREATE RECTANGULAR MESH
        problem.createRectMesh(w, h, pow(2, n_mesh), pow(2, n_mesh));

        //_____BOUNDARY CONDITION SELECTION_____//
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
                    if (face->position().y() > h - 1e-7 && face->isBoundary())
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
                    if (face->position().x() > w - 1e-7 && face->isBoundary())
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
    }
   timer.tac(true, "building mesh");
    //______MATERIAL PROPERTIES_____//
    problem.addConstantMaterial(density, 1e-5, conductivity, specificHeat);
    problem.assignMaterial();
    
    //______BOUNDARY CONDITIONS_____//
    problem.addConstTempBoundary(300);    //BOTTOM
    problem.addConstFluxBoundary(0);       //TOP
    problem.addConstTempBoundary(600);    //LEFT
    problem.addConstTempBoundary(800);   //RIGHT

    problem.assignBoundaryCondition(0, 0);
    problem.assignBoundaryCondition(1, 1);
    problem.assignBoundaryCondition(2, 2);
    problem.assignBoundaryCondition(3, 3);


    //______INITIALIZATION_____//
    field::ScalarStateVector initialScalars;
    initialScalars.pressure = 101325; //Pa
    initialScalars.temperature = temperature; //K
    problem.initializeFields(velocity, initialScalars);


    //______CONFIG EQUATIONS_____//
    problem.initEquations();
    problem.configConvectiveTerm(faceInterpolation);
    problem.configDiffusiveTerm(gradientFlux);

    problem.buildProblem();
    

    //______MATRIX BUILDERS_____//
    solver::ParalelMatrixBuilder paralelBuilder(4);
    solver::MatrixBuilder serialBuilder;
    if (!temporal)
    {
        //______STATIC SOLVER_____//
        solver::StationarySolver sSolver(&serialBuilder);
        double meanSolverTime = 0;

        timer.tic();
        sSolver.solve(&problem);
        timer.tac(true, "Solving problem");
        //______SAVE CONTOUR_____//
        post::Contour contour;
        contour.setProblem(&problem);
        timer.tic();
        contour.saveContourFile("FaceInterpolationComparative//Contour_Temperature_Stationary_" + faceInterpolation->name() + "_" + mesh_name + "_" + std::to_string(n_cell), &sSolver.solution(), false);
        timer.tac(true, "Saving Results");

        //return;

        timer.tic();
        mesh::BoundingBox boundingBox = problem.mesh()->boundingBox();
        std::cout << boundingBox.size << std::endl;
        post::CellSampledLine plot_line(vector2d(boundingBox.size.x() / 2 - 1e-10, 0), vector2d(boundingBox.size.x() / 2 - 1e-10, boundingBox.size.y()), 1e-5);
        //post::CellSampledLine plot_line(vector2d(0, boundingBox.size.y() / 2), vector2d(boundingBox.size.x(), boundingBox.size.y() / 2), 1e-5);
        plot_line.sampleSolution(&problem, sSolver.solution(), false);
        plot_line.saveResults("FaceInterpolationComparative//Center_Line_Temperature_Stationary_" + faceInterpolation->name() + "_" + mesh_name + "_" + std::to_string(n_cell));
        timer.tac(true, "SAVING LINE");
        return;
    }


    else
    {
        //TEMPORAL SOLVER
        timestep::FixedTimeStep timeStep(0.05);
        solver::stop::StopAtStep stoppingCriteria(3);
        solver::TemporalSolver solver(&serialBuilder);
        solver::FixedStepActivity printTimeStep(1);
        solver::FixedStepActivity* pPrint = &printTimeStep;
        printTimeStep.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field)
            {
                    std::cout << "Step number: " << pPrint->step() << ", actual time: " << time << std::endl;
            });
        solver.addActivity(&printTimeStep);


        //CONTOUR SAVER CREATION
        post::Contour solutionSaver;
        std::string file_name = "temperature_contour";
        solutionSaver.setProblem(&problem);
        solutionSaver.initfile(file_name);
        int currentIteration = 0;
        double spacingFactor = 0.03;
        solver::FixedStepActivity saveContour(1);
        solutionSaver.saveSnapshot(problem.fields()->scalarField(field::temperature));
        saveContour.setAction([&](double time, double dt, sys::Problem* problem, field::Fields* field) {
            currentIteration++;
            std::cout << "SAVING SOLUTION..." << std::endl;
            vector2d box = problem->mesh()->boundingBox().size;
            
            vector2d offset((double)currentIteration* (1 + spacingFactor)* box.x(),0);
            solutionSaver.saveSnapshot(field->scalarField(field::temperature),offset);
            });
        solver.addActivity(&saveContour);

        timer.tic();
        //SOLVER
        solver.solve(&problem, &timeStep, &stoppingCriteria, propagator);
        timer.tac(true, "SIMULATION") / 100;
        solutionSaver.save_contour(10, 1080, 720);

    }


}

int main()
{

    std::unique_ptr<term::FaceInterpolation> uds = std::make_unique<term::UDS>();
    std::unique_ptr<term::FaceInterpolation> cds = std::make_unique<term::CDS>();
    std::unique_ptr<term::FaceInterpolation> powerLaw = std::make_unique<term::PowerLaw>();
    std::unique_ptr<term::FaceInterpolation> secondOrderUpwind = std::make_unique<term::SecondOrderUpWind>();
    //Gradient Flux
    std::unique_ptr<term::GradientFlux> centralDiffGrad = std::make_unique<term::CentralDifferenceGradient>();
    std::unique_ptr<term::GradientFlux> orthogonalCorrected = std::make_unique<term::OrthogonalCorrectedGradient>();

    std::unique_ptr <solver::Propagator> eulerImplicid = std::make_unique<solver::EulerImplicit>();
    std::unique_ptr <solver::Propagator> crankNicolson = std::make_unique<solver::CrankNicolson>();
    double density = 1.225;
    double conductivity = 0.025;
    double specificHeat = 1004.5;
    double initialTemperature = 500;
    vector2d velocity(0, 0.1);

    std::vector<double> timeTriUDS;
    std::vector<double> timeTriCDS;
    std::vector<double> timeTriPL;
    std::vector<double> timeTriSOUP;

    std::vector<double> timeQuadUDS;
    std::vector<double> timeQuadCDS;
    std::vector<double> timeQuadPL;
    std::vector<double> timeQuadSOUP;
    
    double timeMesh;
    double timeSimulation;
    int times = 1;
    tool::Chronometer timer;
    timer.tic();
    solveProjectProblem(false, 6, uds.get(), centralDiffGrad.get(), crankNicolson.get(), density, conductivity, specificHeat, velocity, initialTemperature, true);

    timer.tac(true, "TIME ALL SIMULATIONS");


}




    

