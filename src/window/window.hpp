#pragma once

#include <stdexcept>
#include <string>
#include <SDL2/SDL.h>

class Window {
  public:
    Window();
    ~Window();

    void clear();
    void swap();

  private:
    SDL_Window* window = nullptr;
    SDL_GLContext context;
};
