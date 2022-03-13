/*****************************************************************//**
 * \file   mesh.h
 * \brief  
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once
//Think about the use of vector or list as cointainer for mesh data
// vector->fast random access
// list->fast insertion and deletion

//STL
#include <vector>

//CFD
//---BASE
#include <base/global_typedef.h>
//---MESH
#include <mesh/mesh_typedefs.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>
#include <mesh/base/node.h>
#include <mesh/reader/mesh_loader.h>

namespace mesh {


/**
 * @brief Mesh Container (Usually Big Size) managed internally with smart pointers.
 * Mesh needs to be loaded using a loader helper class in with the cell and node
 * extract info is provided. After calling loadMesh succesfully, the mesh is ready to
 * be used
 */
class Mesh
{
   
public:
    Mesh();
    ~Mesh();

    void loadMesh(MeshLoader *loader);
    size_t getMemorySize();
    pMeshNodes& nodes() { return _nodes; }
    pMeshCells& cells() { return _cells; }
    pMeshFaces& faces() { return _faces; }
private:
    pMeshNodes _nodes;
    pMeshCells _cells;
    pMeshFaces _faces;


    /**
     * @brief Builds the mesh with node and cell data, creates the faces and
     * calculates internal geometry data of cells(centroid) and
     * faces(centroid and normals)
     * 
     */
    void buildMesh();

    /**
     * @brief Create faces data from mesh cells and nodes. For >3 nodes faces are built following the index order
     * for example: cell:[0,1,2,3] faces:[0-1,1-2,2-3,3-0]
     * 
     */
    void createFaces();
};



}