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

namespace mesh
{
class Node
{

public:
	Node(vector2d _pos_);
	vector2d pos() { return _pos; }

	void modify(vector2d _newpos_);
	~Node()
	{
		/*if (env::show_debug_print__)
		{
			std::cout << "Node deleted" << std::endl;
		}*/

	}
private:
	//size_t _index;	//index that represents the node (UNIQUE)
	vector2d _pos;		//node x and y coordinates in meters
};


}

