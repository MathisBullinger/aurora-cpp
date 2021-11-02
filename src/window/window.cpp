#include "window.hpp"
#include <GL/glew.h>

using namespace std;

namespace aur {

Window::Window() {
  if (SDL_Init(SDL_INIT_VIDEO)) throw runtime_error(string("couldn't init SDL: ") + SDL_GetError());

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  window = SDL_CreateWindow("Aurora", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
  if (!window) throw runtime_error(string("couldn't open window: ") + SDL_GetError());

  context = SDL_GL_CreateContext(window);
  if (!context) throw runtime_error(string("couldn't create opengl context: ") + SDL_GetError());

  glewExperimental = GL_TRUE;
  auto glew_status = glewInit();
  if (glew_status != GLEW_OK) 
    throw runtime_error(std::string("couldn't init glew: ") + reinterpret_cast<const char*>(glewGetErrorString(glew_status)));

  glClearColor(1.f, 1.f, 1.f, 1.f);
}

Window::~Window() {
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Window::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swap() {
  SDL_GL_SwapWindow(window);
}

}
