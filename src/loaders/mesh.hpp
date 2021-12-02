#pragma once

#include <string>
#include <vector>
#include <map>
#include "renderer/material.hpp"
#include "./objParser.hpp"

namespace aur::loader::mesh {

bool loadOBJ(
  const std::string& path,
  std::vector<float>& vertices,
  MTLMap& mtlMap
);

}
