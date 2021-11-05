#include "./mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>

namespace aur::loader {

using VertexIndex = std::array<unsigned int, 2>;

class OBJParser {
public:
  OBJParser(
    std::ifstream& stream, 
    std::vector<float>& vertices,
    std::vector<unsigned int>& indices
  ): file{stream}, vertices{vertices}, indices{indices} {};

  void parse() {
    for (char c; file.get(c);) {
      if (c == '\n' || c == '#' || isspace(c)) {
        if (state != SKIP && token.length()) digest();
        if (c == '#') state = SKIP;
        else if (c == '\n') state = NEW;
        token = "";
      }
      else token += c;
    }

    for (const auto& vertex : vertexIndices)
      indices.push_back(getVertex(vertex));
  }

private:
  void digest() {
    if (state == NEW) {
      if (OBJParser::types.find(token) == OBJParser::types.end()) {
        std::cout << "ignore unknown type " << token << std::endl;
        state = SKIP;
      }
      else {
        state = OBJParser::types.at(token);
        elc = 0;
      }
    }

    else if (state == V) {
      positions.push_back(number<float>(token));
      if (++elc == 3) state = SKIP;
    }

    else if (state == VN) {
      normals.push_back(number<float>(token));
      if (++elc == 3) state = SKIP;
    }

    else if (state == F) {
      auto [v, vt, vn] = face();
      vertexIndices.push_back({ v - 1, vn - 1 });
      if (++elc == 3) state = SKIP;
    }
  }

  std::map<VertexIndex, unsigned int> vertexMap;

  unsigned int getVertex(const VertexIndex& vertex) {
    if (!vertexMap.contains(vertex)) {
      vertexMap.insert({ vertex, vertices.size() / 6 });
      std::cout << "new vertex: " << vertex[0] << ", " << vertex[1] << std::endl;
      vertices.push_back(positions[vertex[0] * 3]);
      vertices.push_back(positions[vertex[0] * 3 + 1]);
      vertices.push_back(positions[vertex[0] * 3 + 2]);
      vertices.push_back(normals[vertex[1] * 3]);
      vertices.push_back(normals[vertex[1] * 3 + 1]);
      vertices.push_back(normals[vertex[1] * 3 + 2]);
    }
    return vertexMap.at(vertex);
  }

  template <typename T>
  T number(std::string& str) {
    if (str.size() == 0 || (str[0] < '0' || str[0] > '9') && str[0] != '-') return 0;
    return std::stod(str);
  }

  std::array<unsigned int, 3> face() {
    std::array<unsigned int, 3> inds{0, 0, 0};
    std::string seg;
    unsigned int i = 0;

    for (auto c : token + '/') {
      if (c == '/') {
        inds[i++] = number<unsigned int>(seg);
        seg = "";
        if (i == 3) break;
      } 
      else seg += c;
    }
    return inds;
  }

  std::ifstream& file;
  std::string token;

  enum State { NEW, SKIP, V, VN, F };
  State state = NEW;
  unsigned int elc = 0;

  std::vector<float>& vertices;
  std::vector<unsigned int>& indices;
  static const std::map<std::string, State> types;

  std::vector<float> positions;
  std::vector<float> normals;
  std::vector<VertexIndex> vertexIndices;
};

const std::map<std::string, OBJParser::State> OBJParser::types = {
  { "v", V },
  { "vn", VN },
  { "f", F }
};


namespace mesh {

bool loadOBJ(const std::string& path, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return false;
  }
  OBJParser parser(file, vertices, indices);
  parser.parse();
  return true;
}

}


}
