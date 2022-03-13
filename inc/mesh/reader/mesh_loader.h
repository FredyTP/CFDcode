#pragma once

//STL
#include <memory>

//CFD
#include <mesh/mesh_typedefs.h>


namespace mesh
{

class MeshLoader
{
public:

    virtual void createMeshData() = 0;
    void newMesh() {
        _cells = std::make_unique<meshCells>();
        _nodes = std::make_unique<meshNodes>();
    }
    virtual pMeshNodes extractNodes() final
    {
        return std::move(_nodes);
    }
    virtual pMeshCells extractCells() final
    {
        return std::move(_cells);
    }
protected:
    pMeshNodes _nodes;
    pMeshCells _cells;
};


}


