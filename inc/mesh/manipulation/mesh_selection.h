#pragma once
/*****************************************************************//**
 * \file   mesh_selection.h
 * \brief  
 * 
 * \author alfre
 * \date   April 2022
 *********************************************************************/

//STL
#include <vector>

//CFD
#include <mesh/mesh.h>
#include <base/global_typedef.h>
#include <mesh/reader/mesh_reader.h>
namespace mesh
{
    template<class base_mesh>
    class MeshSelection
    {
    public:
        MeshSelection(){}

        //----MANIPULATION----//
        void clearSelection()
        {
            _selection.clear();
        }

        void copyInto(std::vector<base_mesh*>& to_copy) const {
            to_copy = _selection;
        }

        //----DO SOMETHING WITH SELECTION----//
        void for_each(std::function<void(base_mesh*)> element_operation)
        {
            for (auto element : _selection)
            {
                element_operation(element);
            }
        }
        void for_each_const(std::function<void(const base_mesh*)> const_operation) const
        {
            for (auto element : _selection)
            {
                const_operation(element);
            }
        }

        //------TO CREATE SELECTION----//
        void selectFromMesh(const Mesh* mesh,std::function<bool(base_mesh*)> condition)
        {
            clearSelection();
            _mesh = mesh;
            if (_mesh == nullptr)
            {
                return;
            }
            
            auto &elements = mesh->meshElements<base_mesh>();
            for (auto& element : *elements)
            {
                if (condition(element.get()))
                {
                    _selection.push_back(element.get());
                }
            }
        }
        void selectAll(const Mesh* mesh)
        {
            this->selectFromMesh(mesh, [](base_mesh* element) {return true; });
        }
        void selectWithReader(const Mesh* mesh, reader::MeshElementReader<base_mesh> *element_reader)
        {
            clearSelection();
            _mesh = mesh;
            if (_mesh == nullptr)
            {
                return;
            }
            element_reader->read(_mesh, _selection);
        }
        
        

        //----HELPER FUNCTIONS TO CREATE SELECTIONS----//

        //-----GREATER THAN----//
        static std::function<bool(base_mesh*)> xGreaterThan(double value, bool greater = true)
        {
            return [value,greater](base_mesh* elem)
            {
                if (elem->position().x() >= value)
                {
                    return greater;
                }
                else
                {
                    return !greater;
                }
            };
        }
        static std::function<bool(base_mesh*)> yGreaterThan(double value, bool greater = true)
        {
            return[value, greater](base_mesh* elem)
            {
                if (elem->position().y() >= value)
                {
                    return greater;
                }
                else
                {
                    return !greater;
                }
            };
        }

        //----CIRCLE SELECTION----//
        static std::function<bool(base_mesh*)> circleSelection(vector2d center, double radius, bool inside = true)
        {
            return[center, radius, inside](base_mesh* elem)
            {
                vector2d vec = center - elem->position();
                if (vec.squaredNorm() <= radius * radius)
                {
                    return inside;
                }
                else
                {
                    return !inside;
                }
            };
        }
        const Mesh* mesh() const { return _mesh; }
    private:
        const Mesh* _mesh = nullptr;
        std::vector<base_mesh*> _selection;

    };

}
