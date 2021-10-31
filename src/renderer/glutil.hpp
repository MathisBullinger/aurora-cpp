#pragma once

#include <GL/glew.h>
#include <signal.h>
#include <iostream>
#include <sstream>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLC(x) glllog::clear();\
  x;\
  ASSERT(glllog::call(#x, __FILE__, __LINE__))

namespace glllog {

static void clear() {
  while (glGetError());
}

static bool call(const char* fun, const char* file, unsigned int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL error]: (0x" << std::hex << error << std::dec << "): " << fun << " in " << file << ", line " << line << std::endl;
    return false;
  }
  return true;
}

}
