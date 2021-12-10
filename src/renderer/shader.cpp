#include "shader.hpp"
#include "util/gl.hpp"
#include "util/path.hpp"

namespace aur {

unsigned int Shader::currentProgram = 0;
std::map<ShaderName, Shader*> Shader::shaderMap;

Shader::Shader(const std::string& vert_path, const std::string& frag_path) 
: program(glCreateProgram()) {
  unsigned int vt = compile(GL_VERTEX_SHADER, read(vert_path));
  unsigned int ft = compile(GL_FRAGMENT_SHADER, read(frag_path));
  GLC(glAttachShader(program, vt));
  GLC(glAttachShader(program, ft));

  GLC(glLinkProgram(program));
  GLC(glValidateProgram(program));

  GLC(glDetachShader(program, vt));
  GLC(glDetachShader(program, ft));
  GLC(glDeleteShader(vt));
  GLC(glDeleteShader(ft));
}

Shader::~Shader() {
  GLC(glDeleteProgram(program));
}

void Shader::use() {
  if (currentProgram == program) return;
  currentProgram = program;
  GLC(glUseProgram(program));
}

unsigned int Shader::compile(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  GLC(glShaderSource(id, 1, &src, nullptr));
  GLC(glCompileShader(id));

  int result;
  GLC(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    GLC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* msg = (char*)alloca(length * sizeof(char));
    GLC(glGetShaderInfoLog(id, length, &length, msg));
    throw std::runtime_error("failed to compile " + type2Str(type) + " shader: " + msg);
  }

  return id;
}

std::string Shader::read(const std::string& path) {
  std::ifstream input(path);
  if (!input) throw std::runtime_error("couldn't read path "+ path);
  std::stringstream ss;
  ss << input.rdbuf();
  return ss.str();
}

std::string Shader::type2Str(GLenum type) {
  if (type == GL_VERTEX_SHADER) return "vertex";
  if (type == GL_FRAGMENT_SHADER) return "fragment";
  if (type == GL_GEOMETRY_SHADER) return "geometry";
  return "unknown";
}

Shader* Shader::get(const std::string& vertex, const std::string& fragment) {
  if (!shaderMap.contains({vertex, fragment})) 
    shaderMap.insert({{vertex, fragment}, new Shader(
      path::join(path::SHADERS, vertex),
      path::join(path::SHADERS, fragment)
    )});

  return shaderMap.at({vertex, fragment});
}

void Shader::deleteShaders() {
  for (const auto& v : shaderMap) delete v.second;
}

int Shader::getUniform(const std::string& name) {
  if (uniformIds.contains(name)) return uniformIds[name];
  return uniformIds[name] = glGetUniformLocation(program, name.c_str());
}

void Shader::setUniform(const std::string& name, const Matrix<4, 4, float>& matrix) {
  assert(currentProgram == program);
  GLC(glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &matrix.values[0]));
}

void Shader::setUniform(const std::string& name, const Matrix<3, 3, float>& matrix) {
  assert(currentProgram == program);
  GLC(glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &matrix.values[0]));
}

void Shader::setUniform(const std::string& name, const vec3<float>& vector) {
  assert(currentProgram == program);
  GLC(glUniform3fv(getUniform(name), 1, &vector.values[0]));
}

void Shader::setUniform(const std::string& name, float x, float y, float z) {
  setUniform(name, vec3<float>{ x, y, z});
}

void Shader::setUniform(const std::string& name, float n) {
  assert(currentProgram == program);
  GLC(glUniform1f(getUniform(name), n));
}

void Shader::setUniform(const std::string& name, bool v) {
  assert(currentProgram == program);
  GLC(glUniform1i(getUniform(name), (int)v));
}

void Shader::setUniform(const std::string& name, int n) {
  assert(currentProgram == program);
  GLC(glUniform1i(getUniform(name), n));
}

void Shader::setUniformArr(const std::string& name, std::vector<float> values) {
  assert(currentProgram == program);
  GLC(glUniform1fv(getUniform(name), values.size(), &values[0]));
}

}
