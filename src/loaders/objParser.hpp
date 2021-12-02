#pragma once

#include <string>
#include <map>
#include <array>
#include <fstream>
#include <vector>
#include "renderer/material.hpp"

namespace aur::loader::mesh {

using VertexIndex = std::array<unsigned int, 3>;
using MTLMap = std::map<Material*, std::vector<unsigned int>>;

struct Face {
  std::vector<VertexIndex> triangulate() const;
  std::vector<VertexIndex> vertices;
};

class OBJParser {
public:
  OBJParser(
    const std::string& id,
    std::ifstream& stream, 
    std::vector<float>& vertices,
    MTLMap& mtlMap
  );

  void parse();

private:
  void digest();

  std::map<VertexIndex, unsigned int> vertexMap;

  unsigned int getVertex(const VertexIndex& vertex);

  template <typename T>
  T number(std::string& str) {
    if (str.size() == 0 || (str[0] < '0' || str[0] > '9') && str[0] != '-') return 0;
    return std::stod(str);
  }

  std::array<unsigned int, 3> face();

  std::ifstream& file;
  std::string token;

  enum State { NEW, SKIP, V, VN, VT, F, MTL };
  State state = NEW;
  unsigned int elc = 0;

  std::vector<float>& vertices;
  MTLMap& mtlMap;
  static const std::map<std::string, State> types;

  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;

  std::vector<Face>& getFaces(const std::string& material);

  std::map<std::string, std::vector<Face>> materialFaces;
  std::string material = "none";
  const std::string id;
};

}
