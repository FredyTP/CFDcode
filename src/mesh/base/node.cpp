#include <mesh/base/node.h>


namespace mesh
{
	


Node::Node(size_t _index_, vector2d _pos_) : _index(_index_), _pos(_pos_)
{
	if (env::show_debug_print__)
	{
		std::cout << "Created Node" << std::endl;
	}
}

void Node::modify(vector2d _newpos_)
{
	//_index = _newindex_;
	_pos = _newpos_;
}

}
