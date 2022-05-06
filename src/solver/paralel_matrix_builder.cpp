#include <solver/matrix/paralel_matrix_builder.h>

#include <thread>
void solver::ParalelMatrixBuilder::buildSystem(const sys::Problem* problem)
{
    size_t num_equation = problem->mesh()->cells()->size();
    size_t num_faces_int = problem->mesh()->internalFaces().size();
    size_t num_faces_ext = problem->mesh()->boundaryFaces().size();
    matrixes.clear();
    futures.clear();

    for (int i = 0; i < _n_threads; i++)
    {
        matrixes.push_back(num_equation);
        matrixes[i].index = i;
        futures.push_back(std::async(ParalelMatrixBuilder::buildMatrixPart, this, problem, i));     
    }
    for (int i = 0; i < _n_threads; i++)
    {
        futures[i].get()->moveInto(&matrixes[i]);
    }
    //BUILD BOUNDARY CONDITION
    buildMatrixBoundaryPart(this, &matrixes[0], problem, 0, 1);
    _independent.resize(num_equation);
    _independent.setZero();
    std::list<Eigen::Triplet<double>> matrix_triplets;
    std::list<Eigen::Triplet<double>> volume_triplets;
    for (int i = 0; i < matrixes.size(); i++)
    {        
        matrix_triplets.splice(matrix_triplets.end(), *matrixes[i].cellCoeficients());
        volume_triplets.splice(volume_triplets.end(), *matrixes[i].cellDotCoeficients());
        
        _independent += *matrixes[i].coeficients();
    }

    systemMatrix.data().clear();
    systemMatrix.resize(num_equation, num_equation);
    systemMatrix.setFromTriplets(matrix_triplets.begin(), matrix_triplets.end(),
        [](double phi1, double phi2) {
        return phi1 + phi2;
    });   
    _volumeMatrix.data().clear();
    _volumeMatrix.resize(num_equation, num_equation);
    _volumeMatrix.setFromTriplets(volume_triplets.begin(), volume_triplets.end(),
        [](double phi1, double phi2) {
        return phi1 + phi2;
    });
}

std::unique_ptr<math::SystemSubmatrix> solver::ParalelMatrixBuilder::buildMatrixPart(ParalelMatrixBuilder* builder, const sys::Problem* problem, int part)
{
    
    
    size_t num_equation = problem->mesh()->cells()->size();
    size_t num_faces_int = problem->mesh()->internalFaces().size();
    size_t num_faces_ext = problem->mesh()->boundaryFaces().size();

    std::unique_ptr<math::SystemSubmatrix> submatrix = std::make_unique<math::SystemSubmatrix>(num_equation);
    
    double step_eq = (double)num_equation / builder->_n_threads;
    double step_ext = (double)num_faces_ext / builder->_n_threads;
    double step_int = (double)num_faces_int / builder->_n_threads;
    int internal_begin = ceil(step_int * part);
    int internal_end = floor(step_int * (part+1));

    int external_begin = ceil(step_ext * part);
    int external_end = floor(step_ext * (part+1));

    int cell_begin = ceil(step_eq * part);
    int cell_end = floor(step_eq * (part+1));


    buildMatrixInternalPart(builder, submatrix.get(), problem, internal_begin, internal_end);
    buildMatrixExternalPart(builder, submatrix.get(), problem, external_begin, external_end);
    buildMatrixTemporalTerm(builder, submatrix.get(), problem, cell_begin, cell_end);
    
    return std::move(submatrix);
     
}

void solver::ParalelMatrixBuilder::buildMatrixInternalPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end)
{
    for (int i = begin; i < end; i++)
    {
        auto internal_face = problem->mesh()->internalFaces().at(i);
        problem->for_each_faceTerm(
            [submatrix, internal_face, problem](const term::FaceEquationTerm* term)
            {
                term->calculateBothCell(submatrix, internal_face, problem->fields());
            });
    }
}

void solver::ParalelMatrixBuilder::buildMatrixExternalPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end)
{
    for (int i = begin; i < end; i++)
    {
        auto boundaryFace = problem->mesh()->boundaryFaces().at(i);
        problem->for_each_faceTerm(
            [submatrix, boundaryFace, problem](const term::FaceEquationTerm* term)
            {
                term->calculateOneCell(submatrix, boundaryFace, problem->fields());
            });
    }
}

void solver::ParalelMatrixBuilder::buildMatrixBoundaryPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end)
{
    problem->for_each_boundary([submatrix, problem](const bc::BoundaryCondition* boundaryCondition)
    {
    boundaryCondition->calculateBoundaryCondition(submatrix, problem->fields(),0);
    });
}

void solver::ParalelMatrixBuilder::buildMatrixTemporalTerm(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end)
{
    for (int i = begin; i < end; i++)
    {
        auto cell = problem->mesh()->cells()->at(i).get();
        problem->termporalTerm()->calculateCell(submatrix, cell, problem->fields());
    }
}
