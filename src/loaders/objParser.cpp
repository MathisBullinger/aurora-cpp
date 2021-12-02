#include "./objParser.hpp"
#include "./mtlParser.hpp"

namespace aur::loader::mesh {

const std::map<std::string, OBJParser::State> OBJParser::types = {
  { "v", V },
  { "vn", VN },
  { "vt", VT },
  { "f", F },
  { "usemtl", MTL },
  { "mtllib", MTLLIB }
};

OBJParser::OBJParser(
  const std::string& path,
  std::ifstream& stream, 
  std::vector<float>& vertices,
  MTLMap& mtlMap
): file{stream}, vertices{vertices}, mtlMap{mtlMap}, path{path} {};

void OBJParser::parse() {
  for (char c; file.get(c);) {
    if (c == '\n' || c == '#' || isspace(c)) {
      if (state != SKIP && token.length()) digest();
      if (c == '#') state = SKIP;
      else if (c == '\n') state = NEW;
      token = "";
    }
    else token += c;
  }

  if (!mtlLib.contains("none")) mtlLib.insert({ "none", Material::get("none") });
  for (auto& [m, faces] : materialFaces) {
    assert(mtlLib.contains(m));
    auto material = mtlLib.at(m);

    if (!mtlMap.contains(material)) mtlMap.insert({ material, {} });
    for (auto& face : faces)
      for (auto vertex : face.triangulate())
        mtlMap.at(material).push_back(getVertex(vertex));
  }
}

void OBJParser::digest() {
  if (state == NEW) {
    if (OBJParser::types.find(token) == OBJParser::types.end()) {
      state = SKIP;
    }
    else {
      state = OBJParser::types.at(token);
      if (state == F) getFaces(material).push_back({});
      elc = 0;
    }
  }

  else if (state == V) {
    positions.push_back(number<float>(token));
    if (++elc >= 3) state = SKIP;
  }

  else if (state == VN) {
    normals.push_back(number<float>(token));
    if (++elc >= 3) state = SKIP;
  }

  else if (state == VT) {
    uvs.push_back(number<float>(token));
    if (++elc >= 2) state = SKIP;
  }

  else if (state == F) {
    auto [v, vt, vn] = face();
    getFaces(material).back().vertices.push_back({ v, vn, vt });
  }

  else if (state == MTL) {
    material = token;
  }

  else if (state == MTLLIB) {
    auto mtlPath = path;
    while (mtlPath.size() && mtlPath.back() != '/') mtlPath.pop_back();
    mtlLib = parseMTL(mtlPath + token);
  }
}

unsigned int OBJParser::getVertex(const VertexIndex& vertex) {
  if (!vertexMap.contains(vertex)) {
    vertexMap.insert({ vertex, vertices.size() / 8 });
    vertices.push_back(positions[vertex[0]]);
    vertices.push_back(positions[vertex[0] + 1]);
    vertices.push_back(positions[vertex[0] + 2]);
    vertices.push_back(normals[vertex[1]]);
    vertices.push_back(normals[vertex[1] + 1]);
    vertices.push_back(normals[vertex[1] + 2]);
    vertices.push_back(uvs.size() > vertex[2] ? uvs[vertex[2]] : 0);
    vertices.push_back(uvs.size() > vertex[2] + 1 ? uvs[vertex[2] + 1] : 0);
  }
  return vertexMap.at(vertex);
}

std::vector<Face>& OBJParser::getFaces(const std::string& material) {
  if (!materialFaces.contains(material)) materialFaces.insert({material, {}});
  return materialFaces.at(material);
}

std::array<unsigned int, 3> OBJParser::face() {
  std::array<unsigned int, 3> inds{0, 0, 0};
  std::string seg;
  unsigned int i = 0;

  for (auto c : token + '/') {
    if (c == '/') {
      auto n = number<int>(seg);
      if (n > 0) n--;
      n *= i == 1 ? 2 : 3;
      if (n < 0)  n = (i == 0 ? positions : i == 1 ? uvs : normals).size() + n;
      inds[i++] = n;
      seg = "";
      if (i == 3) break;
    } 
    else seg += c;
  }
  return inds;
}

std::vector<VertexIndex> Face::triangulate() const {
  if (vertices.size() == 3) return vertices;
  if (vertices.size() == 4) return {
    vertices[0],
    vertices[1],
    vertices[2],
    vertices[2],
    vertices[3],
    vertices[0]
  };
  assert(false);
}

}
