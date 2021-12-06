#include "./mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>
#include "util/path.hpp"

namespace aur::loader::mesh {

using TangentMap = std::map<unsigned int, std::array<Vector<3, float>, 2>>;

void calcTangents(
  const std::vector<float>& vertices, 
  const std::vector<unsigned int>& indices,
  TangentMap& tangents,
  unsigned int stride = 8
) {
  for (unsigned int i = 0; i < indices.size(); i += 3) {
    Vector<3, float> pos[3];
    Vector<2, float> uvs[3];

    for (unsigned int e = 0; e < 3; e++) {
      auto j = indices[i + e] * stride;
      pos[e] = { vertices[j], vertices[j + 1], vertices[j + 2] };
      uvs[e] = { vertices[j + 6], vertices[j + 7] };
    }

    Vector<3, float> dPos1 = pos[1] - pos[0];
    Vector<3, float> dPos2 = pos[2] - pos[0];
    Vector<2, float> dUV1 = uvs[1] - uvs[0];
    Vector<2, float> dUV2 = uvs[2] - uvs[0];

    float r = 1.f / (dUV1.x() * dUV2.y() - dUV1.y() * dUV2.x());
    Vector<3, float> tangent = (dPos1 * dUV2.y() - dPos2 * dUV1.y()) * r;
    Vector<3, float> bitangent = (dPos2 * dUV1.x() - dPos1 * dUV2.x()) * r;
    
    for (unsigned int e = 0; e < 3; e++) {
      if (!tangents.contains(indices[i + e]))
        tangents.insert({ indices[i + e], { tangent, bitangent } });
    }
  }
}

bool loadOBJ(const std::string& path, std::vector<float>& vertices, MTLMap& mtlMap) {
  auto comPath = path::join(path::MESHES, path);
  std::ifstream file(comPath);
  if (!file.is_open()) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return false;
  }
  std::vector<float> incomplete;
  OBJParser parser(comPath, file, incomplete, mtlMap);
  parser.parse();
  file.close();

  TangentMap tangents;
  for (const auto& [_, indices] : mtlMap) calcTangents(incomplete, indices, tangents);
  
  for (unsigned int i = 0; i < incomplete.size() / 8; i++) {
    assert(tangents.contains(i));

    for (unsigned int e = 0; e < 8; e++) vertices.push_back(incomplete[i * 8 + e]);
    const auto& [tangent, bitangent] = tangents[i];
    vertices.push_back(tangent[0]);
    vertices.push_back(tangent[1]);
    vertices.push_back(tangent[2]);
    vertices.push_back(bitangent[0]);
    vertices.push_back(bitangent[1]);
    vertices.push_back(bitangent[2]);
  }
  
  return true;
}

}
