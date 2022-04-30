#include <mesh/mesh.h>

namespace mesh
{


Mesh::Mesh()
{


}

Mesh::~Mesh()
{
}

void Mesh::loadMesh(MeshLoader* loader)
{
    loader->createMeshData();
    _nodes = loader->extractNodes();
    _cells = loader->extractCells();
    this->buildMesh();
}

size_t Mesh::getMemorySize()
{
    return sizeof(Node) * _nodes->size() + sizeof(Cell) * _cells->size() + sizeof(Face) * _faces->size();
}



void Mesh::createFaces() {
    _faces = std::make_unique<meshFaces>();
    std::vector<std::unique_ptr<Face>> alone_faces;

    //Iterate over all cells
    for (size_t cellid = 0; cellid < _cells->size(); cellid++)
    {
        Cell* cell = _cells->at(cellid).get();

        std::vector<Node*> nodes = cell->nodes(); 

        std::vector<std::unique_ptr<Face>> new_faces;
        std::vector<Face*> cell_faces;

        //For each cell iterate over its nodes to create
        //new faces vector in and ordered way:
        //Node1+Node2 // Node2+Node3 // Node3 + Node 1 // <-Tri
        for (int i = 0; i < nodes.size(); i++)
        {
            //Create New Face
            std::unique_ptr<Face> temp_face = std::make_unique<Face>();
            temp_face->setCell1(cell);

            //Nodes must be ordered!!! (no problem on triangles)
            if (i < nodes.size() - 1)
            {
                temp_face->setNodes(nodes[i], nodes[i + 1]);
            }
            else
            {
                temp_face->setNodes(nodes[i], nodes[0]);
            }

            //Add new face to the vector
            new_faces.push_back(std::move(temp_face));
        }

        //Iterate over all faces with only 1 cell (alone_faces)
        for (size_t faceid = 0; faceid < alone_faces.size(); faceid++)
        {
            Face* face = alone_faces.at(faceid).get();

            //Iterate over left new faces
            for (int i = 0; i < new_faces.size(); i++)
            {
                //Check if any of the new faces created exists in the alone_face vector
                if (face->isEqual(new_faces[i].get()))
                {
                    face->setCell2(cell);

                    //Add face ptr to the cell faces list
                    cell_faces.push_back(face);

                    //Add matching face to mesh faces
                    _faces->push_back(std::move(alone_faces.at(faceid)));

                    //Free vector space
                    new_faces.erase(new_faces.begin() + i);
                    alone_faces.erase(alone_faces.begin() + faceid);

                    //Compensate index
                    faceid--;
                    i--;
                }
            }
        }
        //Iterate over unmatched faces and add it to the alone_faces vector
        for (size_t to_add_faceid = 0; to_add_faceid < new_faces.size(); to_add_faceid++)
        {
            auto to_add_face = std::move(new_faces[to_add_faceid]);

            //Add the reference to cell face vector
            cell_faces.push_back(to_add_face.get());

            //Give face ownership to mesh alone faces vector
            alone_faces.push_back(std::move(to_add_face));
        }

        //Set the cell faces references
        cell->setFaces(cell_faces);
        

    }

    //Add unmatched faces to the mesh(boundary faces)
    for (auto& left_face : alone_faces)
    {
        _faces->push_back(std::move(left_face));
    }

    //Config index
    for (size_t index = 0; index < _faces->size(); index++)
    {
        _faces->at(index)->setIndex(index);
    }
}

void Mesh::calculateBoundingBox()
{
    vector2d minpos(1e30,1e30);
    vector2d maxpos(-1e30, -1e30);

    for(auto& node : *_nodes)
    {
        auto pos = node->position();
        if(pos.x() < minpos.x())
        {
            minpos.x() = pos.x();
        }
        if(pos.y() < minpos.y())
        {
            minpos.y() = pos.y();
        }
        if(pos.x() > minpos.x())
        {
            maxpos.x() = pos.x();
        }
        if(pos.y() > minpos.y())
        {
            maxpos.y() = pos.y();
        }
    }
    _boundingBox.leftbot = minpos;
    _boundingBox.size = maxpos - minpos;
}

void Mesh::calculateMaxCellFaces()
{
    _maxCellFaces = 0;
    for (auto& cell : *_cells)
    {
        if (cell->faces().size() > _maxCellFaces)
        {
            _maxCellFaces = cell->faces().size();
        }
    }
}

void Mesh::buildMesh()
{
    this->createFaces();
    _boundaryFaces.clear();
    _internalFaces.clear();
    for (auto& cell : *_cells)
    {
        cell->build();
    }
    for (auto& face : *_faces)
    {
        face->build();
        if (face->isBoundary())
        {
            _boundaryFaces.push_back(face.get());
            this->createBoundaryCell(face.get());
        }
        else
        {
            _internalFaces.push_back(face.get());
        }
    }

    this->calculateBoundingBox();
    this->calculateMaxCellFaces();
}

void Mesh::createBoundaryCell(Face* face)
{
   
    std::unique_ptr<mesh::Cell> new_cell = std::make_unique<mesh::Cell>(-1);

    //Create Connectivity vectors
    std::vector<mesh::Face*> faces;
    faces.push_back(face);

    std::vector<mesh::Node*> nodes;
    nodes.push_back(face->node1());
    nodes.push_back(face->node2());
    new_cell->setNodes(nodes);

    //Set Connectivity info
    new_cell->setFaces(faces);
    face->setCell2(new_cell.get());

    //BUILD GEOMETRY INFO
    new_cell->build();
    face->rebuild();

    this->addCell(new_cell);

}


}
