#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <array>
#include <vector>
#include <map>
#include "math/matrix.hpp"
#include "math/vector.hpp"

namespace aur {

using ShaderName = std::array<const std::string, 2>;

class Shader {
public:
  Shader(const std::string& vert_path, const std::string& frag_path);
  ~Shader();

  void use();

  static Shader* get(const std::string& vertex, const std::string& fragment);
  static void deleteShaders();

  unsigned int getUniform(const std::string& name);
  unsigned int getTexture(const std::string& name);
  
  void setUniform(const std::string& name, const Matrix<3, 3, float>& matrix);
  void setUniform(const std::string& name, const Matrix<4, 4, float>& matrix);
  void setUniform(const std::string& name, const vec3<float>& vector);
  void setUniform(const std::string& name, float x, float y, float z);
  void setUniform(const std::string& name, float n);
  void setUniform(const std::string& name, bool v);
  void setUniform(const std::string& name, int n);
  void setUniform(const std::string& name, unsigned int n);
  void setUniformArr(const std::string& name, std::vector<float> values);

  operator int() const;
  bool operator ==(const Shader& rhs) const;
private:
  const unsigned int program;
  std::map<std::string, unsigned int> uniformIds;
  std::map<std::string, unsigned int> textureIds;

  static unsigned int currentProgram;
  static std::map<ShaderName, Shader*> shaderMap;

  std::string read(const std::string& path);
  unsigned int compile(unsigned int type, const std::string& source);
  std::string type2Str(GLenum type);
};

}
