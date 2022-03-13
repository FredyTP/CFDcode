/*****************************************************************//**
 * \file   mesh_typedefs.h
 * \brief  
 *
 * 
 * \author alfre
 * \date   March 2022
 *********************************************************************/
#pragma once

#include <memory>
#include <mesh/base/node.h>
#include <mesh/base/cell.h>
#include <mesh/base/face.h>

namespace mesh {


typedef std::vector<std::unique_ptr<Node>> meshNodes;
typedef std::vector<std::unique_ptr<Cell>> meshCells;
typedef std::vector<std::unique_ptr<Face>> meshFaces;

typedef std::unique_ptr<meshNodes> pMeshNodes;
typedef std::unique_ptr<meshCells> pMeshCells;
typedef std::unique_ptr<meshFaces> pMeshFaces;

}
