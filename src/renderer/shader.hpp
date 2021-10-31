#pragma once

#include "glutil.hpp"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>

class Shader {
  public:
    Shader(const std::string& vert_path, const std::string& frag_path);
    ~Shader();
    void use();

    const unsigned int program;
  private:
    std::string read(const std::string& path);
    unsigned int compile(unsigned int type, const std::string& source);
    std::string type2Str(GLenum type);
};
