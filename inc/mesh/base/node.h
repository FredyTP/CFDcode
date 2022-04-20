/*****************************************************************//**
 * \file   Node.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

//STL
#include <iostream>

//CFD
#include <base/global_typedef.h>

#include <mesh/base/base_geometry.h>

namespace mesh
{
	class Cell;

class Node : public BaseGeometry
{

public:
	Node(size_t _index_,vector2d _pos_);
	vector2d pos() { return _pos; }

	void modify(vector2d _newpos_);
	size_t index() const { return _index; }
	~Node()
	{
		/*if (env::show_debug_print__)
		{
			std::cout << "Node deleted" << std::endl;
		}*/

	}
private:
	size_t _index;	//index that represents the node (UNIQUE)
	std::vector<Cell*> _cells; //Cells this node is cointained in (for node interpolation)
};


}

