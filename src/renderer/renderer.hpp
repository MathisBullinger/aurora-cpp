#pragma once

#include "shader.hpp"
#include <GL/glew.h>
#include <string>
#include "glutil.hpp"

class Renderer {
  public:
    Renderer();
    ~Renderer();
    void render();

    void setWireMode(bool on);

  private:
    bool glLogCall(const char* fun, const char* file, unsigned int line);
  
    Shader* shader = nullptr;
};
