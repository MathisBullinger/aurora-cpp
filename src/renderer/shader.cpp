#include "shader.hpp"

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
