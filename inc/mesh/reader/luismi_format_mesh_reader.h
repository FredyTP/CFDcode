#pragma once

//STLL
#include <iostream>
#include <string>
#include <fstream>

//CFD
#include <mesh/mesh.h>
#include <mesh/mesh_typedefs.h>
#include <mesh/reader/mesh_loader.h>

namespace mesh
{


class LuisMiformatMeshReader : public MeshLoader
{
    
public:
    LuisMiformatMeshReader(std::string _nodepath_, std::string _cellpath_)
    {
        this->setPaths(_nodepath_, _cellpath_);
    }
    void setPaths(std::string _nodepath_, std::string _cellpath_)
    {
        _cellpath=_cellpath_;
        _nodepath=_nodepath_;
    }

    void createMeshData()
    {
        MeshLoader::newMesh();
        this->createNodes();
        this->createCells();
    }

    ~LuisMiformatMeshReader()
    {
        
    }
private:

    std::string _cellpath;
    std::string _nodepath;

    //--------CREATE NODES AND CELLS FUNCTIONS------------//
     /**
     * @brief create 2D NODES from file path in
     * _nodepath variable. Only 2D nodes are valid
     * with the following format:
     * #   X  Y 
     * 0.0   0.0   
     * 1.0   0.0 
     * ...
     */
    void createNodes() {
        std::fstream nodefile;
        nodefile.open(_nodepath);
        if (nodefile.is_open())
        {
            //Read file header
            std::string c1, c2, c3;
            nodefile >> c1;
            nodefile >> c2;
            nodefile >> c3;

            //Check file header to check format is valid
            if (c1.compare("#") == 0) //Check file format
            {
                size_t counter = 0;
                std::string snode_x, snode_y;
                double node_x, node_y;
                while (!nodefile.eof()) //Read whole file
                {

                    nodefile >> snode_x; //X coordinate in [m]
                    if (nodefile.eof()) 
                    {
                        break;
                    }
                    nodefile >> snode_y; //Y coordinate in [m]
                    if (nodefile.eof())
                    {
                        break;
                    }

                    //Convert string to double
                    node_x = std::stod(snode_x); 
                    node_y = std::stod(snode_y);

                    //Create and add new node to the list                   
                    _nodes->push_back(std::make_unique<Node>(vector2d({ node_x,node_y })));
                    counter++;

                    //std::cout << counter << ": " << node_x << ", " << node_y << std::endl;
                }
            }

            //Close file
            nodefile.close();
        }


    }

    /**
     * @brief create TRIANGULAR cells from file path in
     * _cellpath variable. Only 2D triangular cells are valid
     * with the following format:
     * #    Point1  Point2  Point3
     * 1   2   3
     * 2   3   4
     * ...
     */
    void createCells() {

        std::fstream cellfile;
        cellfile.open(_cellpath);

        //Check if file succesfully open
        if (cellfile.is_open())
        {

            //Read file header
            std::string c1, c2, c3, c4;
            cellfile >> c1;
            cellfile >> c2;
            cellfile >> c3;
            cellfile >> c4;

            //Check header format
            if (c1.compare("#") == 0)
            {
                size_t counter = 0;
                std::string snode1, snode2, snode3;
                size_t node_index1, node_index2, node_index3;

                //Read whole fille
                while (!cellfile.eof())
                {
                    //This format is only valid for triangle cells!!

                    cellfile >> snode1; //Cell number 1
                    if (cellfile.eof())
                    {
                        break;
                    }
                    cellfile >> snode2; //Cell number 2 
                    if (cellfile.eof())
                    {
                        break;
                    }
                    cellfile >> snode3; //Cell number 3
                    if (cellfile.eof())
                    {
                        break;
                    }

                    //Convert string to index
                    //NOTICE FILE FORMAT INDEX STARTS AT 1
                    //IN C++ INDEX START AT 0 SO DECIDED TO 
                    //USE SAME CONVENTION ON CELLS INDEX
                    node_index1 = std::stoi(snode1) - 1;
                    node_index2 = std::stoi(snode2) - 1;
                    node_index3 = std::stoi(snode3) - 1;

                    //Create new cell
                    auto cell = std::make_unique<Cell>();

                    //Get reference of nodes from index 
                    Node* node1 = _nodes->at(node_index1).get();
                    Node* node2 = _nodes->at(node_index2).get();
                    Node* node3 = _nodes->at(node_index3).get();

                    //Asign them to the cell
                    cell->setNodes({ node1,node2,node3 });

                    //Add new cell to the list
                    _cells->push_back(std::move(cell));


                    counter++;
                    //std::cout << counter << ": " << node_index1 << ", " << node_index2 << ", " << node_index3 << std::endl;
                }
            }
            //Close file
            cellfile.close();
        }
    }
};

}