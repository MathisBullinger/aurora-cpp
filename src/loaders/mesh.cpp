#include "./mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>

namespace aur::loader {

class OBJParser {
public:
  OBJParser(std::ifstream& stream, std::vector<float>& verts, std::vector<unsigned int>& inds): file{stream}, vertices{verts}, vertInd{inds} {};

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
      vertices.push_back(number<float>(token));
      if (++elc == 3) state = SKIP;
    }

    else if (state == F) {
      auto [v, vt, vn] = face();
      vertInd.push_back(v-1);
      if (++elc == 3) state = SKIP;
    }
  }

  template <typename T>
  T number(std::string& str) {
    if (str.size() == 0 || (str[0] < '0' || str[0] > '9') && str[0] != '-') return 0;
    return std::stod(str);
  }

  std::array<int, 3> face() {
    std::array<int, 3> inds{-1, -1, -1};
    std::string seg;
    unsigned int i = 0;

    for (auto c : token + '/') {
      if (c == '/') {
        inds[i++] = number<int>(seg);
        if (i == 3) break;
      } 
      else seg += c;
    }
    return inds;
  }

  std::ifstream& file;
  std::string token;

  enum State { NEW, SKIP, V, F };
  State state = NEW;
  unsigned int elc = 0;

  std::vector<float>& vertices;
  std::vector<unsigned int>& vertInd;
  static const std::map<std::string, State> types;
};

const std::map<std::string, OBJParser::State> OBJParser::types = {
  { "v", V },
  { "f", F }
};


namespace mesh {

bool loadOBJ(
  const std::string& path,
  std::vector<float>& vertices,
  std::vector<float>& uvs,
  std::vector<float>& normals,
  std::vector<unsigned int>& indices
) {
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
