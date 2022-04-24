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

    struct BoundingBox
    {
        vector2d leftbot;
        vector2d size;
    };


/**
 * @brief Mesh Container (Usually Big Size) managed internally with smart pointers.
 * Mesh needs to be loaded using a loader helper class in with the cell and node
 * extract info is provided. After calling loadMesh succesfully, the mesh is ready to
 * be used
 */
class Mesh
{
   //CREATE A MESH BASED CONTAINER WHERE INDEX ARE DONE BY CELLS,NODES,FACES
public:
    Mesh();
    ~Mesh();

    void loadMesh(MeshLoader *loader);
    size_t getMemorySize();
    const pMeshNodes& nodes() const { return _nodes; }
    const pMeshCells& cells() const { return _cells; }
    const pMeshFaces& faces() const { return _faces; }

    const std::vector<Face*>& internalFaces() const { return _internalFaces; }
    const std::vector<Face*>& boundaryFaces() const { return _boundaryFaces; }
    //void addNode(std::unique_ptr<Node>& node){}
    void addCell(std::unique_ptr<Cell>& cell) const { 
        cell->setIndex(_cells->size());
        _cells->push_back(std::move(cell));
    }

    template<class mesh_type>
    const std::unique_ptr<std::vector<std::unique_ptr<mesh_type>>>& meshElements() const
    {
        if constexpr (std::is_same<mesh_type, Node>())
        {
            return _nodes;
        }
        else if constexpr  (std::is_same<mesh_type, Cell>())
        {
            return _cells;
        }
        else if constexpr (std::is_same<mesh_type, Face>())
        {
            return _faces;
        }

        
    }

    const BoundingBox& boundingBox() const { return _boundingBox; }
    //void addFace(std::unique_ptr<Face>& node){}
private:
    //TODO: create class containing 
    //encapsulation of index thing

    pMeshNodes _nodes; 
    pMeshCells _cells;
    pMeshFaces _faces;
    
    std::vector<Face*> _internalFaces;
    std::vector<Face*> _boundaryFaces;

    //GEOMETRIC INFORMATION



    BoundingBox _boundingBox;
    /**
     * \brief Builds the mesh with node and cell data, creates the faces and
     * calculates internal geometry data of cells(centroid) and
     * faces(centroid and normals)
     * 
     */
    void buildMesh();

    /**
     * \brief Adds the boundary "fake" cells to the mesh.
     * 
     * \param face
     */
    void createBoundaryCell(Face* face);

    /**
     * \brief Create faces data from mesh cells and nodes. For >3 nodes faces are built following the index order
     * for example: cell:[0,1,2,3] faces:[0-1,1-2,2-3,3-0]
     * 
     */
    void createFaces();

    /**
     * \brief Calculate mesh bounding box dimensions as size + leftbottom corner.
     * 
     */
    void calculateBoundingBox();

};


}