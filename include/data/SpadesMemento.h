#pragma once

#include <vector>
#include <cassert>
#include <algorithm>
#include <iterator> 
#include <string>

namespace spd
{
    struct SpadesMemento{

        SpadesMemento() = default;
        SpadesMemento(const std::string& data) {
            deserialize(data);
        }

        std::string serialize() const{
            return "";
        }
        void deserialize(const std::string& data){

        }
    };
}