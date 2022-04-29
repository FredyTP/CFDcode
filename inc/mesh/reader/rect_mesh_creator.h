#pragma once

//CFD
#include <mesh/mesh.h>
#include <mesh/mesh_typedefs.h>
#include <mesh/reader/mesh_loader.h>

namespace mesh
{


	class RectMeshCreator : public MeshLoader
	{
	public:
		RectMeshCreator(double w, double h, int w_division, int h_division) : _w(w), _h(h),
			 _w_division(w_division), _h_division(h_division)
		{

		}
		virtual void createMeshData()
		{
			MeshLoader::newMesh();
			int n_a = _w_division + 1;
			int n_b = _h_division + 1;
			double deltaw = _w / _w_division;
			double deltah = _h / _h_division;
			int n_nodes = n_a * n_b;

			int n_cells = (n_a - 1) * (n_b - 1);
			_nodes->resize(n_nodes);
			_cells->resize(n_cells);
			
			for (int i = 0; i < n_a; i++)
			{
				for (int j = 0; j < n_b; j++)
				{
					int index = i + n_a * j;
					vector2d pos(i * deltaw, j * deltah);
					_nodes->at(index) = std::make_unique<mesh::Node>(index, pos);
				}
			}
			for (int i = 0; i < n_a-1; i++)
			{
				for (int j = 0; j < n_b-1; j++)
				{
					int index1 = (i) + (n_a) * (j);
					int index2 = (i) + (n_a) * (j+1);
					int index3 = (i+1) + (n_a) * (j+1);
					int index4 = (i+1) + (n_a) * (j);
					int cellindex = i + (n_a-1) * j;
					auto cell = std::make_unique<mesh::Cell>(cellindex);
					std::vector<mesh::Node*> cellnodes;
					cellnodes.push_back(_nodes->at(index1).get());
					cellnodes.push_back(_nodes->at(index2).get());
					cellnodes.push_back(_nodes->at(index3).get());
					cellnodes.push_back(_nodes->at(index4).get());
					cell->setNodes(cellnodes);

					_cells->at(cellindex) = std::move(cell);
					
				}
			}
		}
	private:
		double _w;
		double _h;
		int _w_division;
		int _h_division;
	};
}