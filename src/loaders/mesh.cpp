#include "./mesh.hpp"
#include <iostream>

namespace aur::loader {

bool Mesh::loadOBJ(
  const std::string& path,
  std::vector<float>& vertices,
  std::vector<float>& uvs,
  std::vector<float>& normals
) {
  auto file = fopen(path.c_str(), "r");
  if (!file) {
    std::cerr << "couldn't read obj file: " << path << std::endl;
    return false;
  }

  std::vector<float> verts_;
  std::vector<float> uvs_;
  std::vector<float> normals_;
  std::vector<int> vertInd, uvInd, normInd;

  while (1) {
    char lineHeader[128];
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF) break;

    if (strcmp(lineHeader, "v") == 0) {
      verts_.resize(verts_.size() + 3);
      fscanf(file, "%f %f %f\n", &verts_[verts_.size() - 3], &verts_[verts_.size() - 2], &verts_[verts_.size() - 1]);
    }
    else if (strcmp(lineHeader, "vt") == 0) {
      uvs_.resize(uvs_.size() + 2);
      fscanf(file, "%f %f\n", &uvs_[uvs_.size() - 2], &uvs_[uvs_.size() - 1]);
    }
    else if (strcmp(lineHeader, "vn") == 0) {
      normals_.resize(normals_.size() + 3);
      fscanf(file, "%f %f %f\n", &normals_[normals_.size() - 3], &normals_[normals_.size() - 2], &normals_[normals_.size() - 1]);
    }
    else if (strcmp(lineHeader, "f") == 0) {
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
      if (matches != 9) {
        std::cerr << "file can't be read" << std::endl;
        return false;
      }
      vertInd.push_back(vertexIndex[0] - 1);
      vertInd.push_back(vertexIndex[1] - 1);
      vertInd.push_back(vertexIndex[2] - 1);
      uvInd.push_back(uvIndex[0] - 1);
      uvInd.push_back(uvIndex[1] - 1);
      uvInd.push_back(uvIndex[2] - 1);
      normInd.push_back(normalIndex[0] - 1);
      normInd.push_back(normalIndex[1] - 1);
      normInd.push_back(normalIndex[2] - 1);
    }
  }

  for (auto i : vertInd) vertices.insert(vertices.end(), verts_.begin() + i * 3, verts_.begin() + i * 3 + 3);
  for (auto i : uvInd) uvs.insert(uvs.end(), uvs_.begin() + i * 2, uvs_.begin() + i * 2 + 2);
  for (auto i : normInd) normals.insert(normals.end(), normals_.begin() + i * 3, normals_.begin() + i * 3 + 3);

  return true;
}

}
