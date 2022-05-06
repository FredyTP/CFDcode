/*****************************************************************//**
 * \file   paralel_matrix_builder.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/
#pragma once

#include <Eigen/Eigen>
#include <vector>
#include <mesh/mesh.h>
#include <boundary/boundary_condition.h>
#include <system/problem.h>
#include <solver/matrix/matrix_builder.h>
#include <future>

namespace solver
{
    /**
     * Builds matrix using n number of threads.
     */
    class ParalelMatrixBuilder : public BaseMatrixBuilder
    {
    public:
        ParalelMatrixBuilder(uint8_t n_threads) : _n_threads(n_threads) {}
        virtual void buildSystem(const sys::Problem* problem);

    private:
        static std::unique_ptr<math::SystemSubmatrix> buildMatrixPart(ParalelMatrixBuilder* builder, const sys::Problem* problem, int part);
        static void buildMatrixInternalPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end);
        static void buildMatrixExternalPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end);
        static void buildMatrixBoundaryPart(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end);
        static void buildMatrixTemporalTerm(ParalelMatrixBuilder* builder, math::SystemSubmatrix* submatrix, const sys::Problem* problem, int begin, int end);
        uint8_t _n_threads;

        std::vector<std::future<std::unique_ptr<math::SystemSubmatrix>>> futures;
        std::vector<math::SystemSubmatrix> matrixes;
    };
}