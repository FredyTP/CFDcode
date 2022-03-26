/*****************************************************************//**
 * \file   vector.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/

//STL VECTOR AS INTERNAL CONTAINER
#include <Eigen\Eigen>
#include <Eigen\Dense>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

/*
namespace mesh
{
	template<typename _Ty,typename _TyGeometry>
	class vector
	{
	public:
		vector() {}
		vector(const std::vector<std::unique_ptr<_TyGeometry>>* meshInfo) { _data.resize(meshInfo->size()); }

		void setZeros() { _data.setZero();}
		_Ty& of(const _TyGeometry* geom)
		{
			return _data(geom->index());
		}

	private:
		Eigen::VectorX<_Ty> _data;
		
	};


	template<typename _Ty>
	using CellVector = vector<_Ty, mesh::Cell>;

	template<typename _Ty>
	using FaceVector = vector<_Ty, mesh::Face>;
	
}

*/
