#pragma once

#include <string>
#include <map>
#include "renderer/material.hpp"

namespace aur::loader::mesh {

std::map<std::string, Material*> parseMTL(const std::string& path);

}
