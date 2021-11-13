#pragma once

#include "./mesh.hpp"
#include <string>
#include <map>
#include <array>
#include <fstream>

namespace aur::loader::mesh {

using VertexIndex = std::array<unsigned int, 3>;

struct Face {
  std::vector<VertexIndex> triangulate() const;
  std::vector<VertexIndex> vertices;
};

class OBJParser {
public:
  OBJParser(
    std::ifstream& stream, 
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices
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

  enum State { NEW, SKIP, V, VN, VT, F };
  State state = NEW;
  unsigned int elc = 0;

  std::vector<float>& vertices;
  std::vector<unsigned int>& indices;
  static const std::map<std::string, State> types;

  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<float> uvs;

  std::vector<Face> faces;
};

}
