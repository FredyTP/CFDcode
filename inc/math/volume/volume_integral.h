/*****************************************************************//**
 * \file   volume_integral.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#include <math/var/system_submatrix.h>
#include <mesh/base/cell.h>
#include <mesh/mesh.h>
#include <field/field.h>
namespace math
{
    namespace volume
    {
        class VolumeIntegral
        {
        public:
            VolumeIntegral(){}
            void integrateMesh(SystemSubmatrix* submatrix, const mesh::Mesh* mesh, const field::Fields* fields)
            {
                auto& cells = mesh->cells();
                for (const auto& cell : *cells.get())
                {
                    double coef = VolumeIntegral::GetVolumeCoef(cell.get(), fields);
                    integrateCell(submatrix, cell.get(), coef);
                }

            }
            void integrateCell(SystemSubmatrix* submatrix, const mesh::Cell* cell, double coef)
            {               
                submatrix->addCellVar(CellVariable(cell->index(),cell->index(), coef * cell->volume()));
            }
            static double GetVolumeCoef(const mesh::Cell* cell, const field::Fields* fields)
            {
                return cell->material()->specificHeat(fields->scalarField(cell)) * fields->scalarField(cell).density;
            }
        };
    }
}
