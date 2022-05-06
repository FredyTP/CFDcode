/*****************************************************************//**
 * \file   named_object.h
 * \brief  
 * 
 * 
 * \author alfre
 * \date   May 2022
 *********************************************************************/

#pragma once
#include <string>
namespace terminal
{
    /**
     * Abstraction for classes that can print its name.
     */
    class NamedObject
    {
    public:
        NamedObject() { _name = "NO_NAME"; }
        NamedObject(const std::string _name_) : _name(_name_) {}
        const std::string& name() { return _name; }
    private:
        std::string _name;
    };
}


