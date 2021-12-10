#pragma once

#include <stdexcept>
#include <string>
#include <SDL2/SDL.h>

namespace aur {

class Window {
public:
  Window();
  ~Window();

  void swap();

private:
  SDL_Window* window = nullptr;
  SDL_GLContext context;
};

}
