#pragma once

#include "shader.hpp"
#include <GL/glew.h>
#include <string>
#include "loaders/texture.hpp"

namespace aur {

class Renderer {
  public:
    Renderer();
    ~Renderer();
    void render();

    void setWireMode(bool on);

  private:
    bool glLogCall(const char* fun, const char* file, unsigned int line);

    unsigned int incr = 0;
    int mvpID;
  
    Shader* shader = nullptr;
};

}
