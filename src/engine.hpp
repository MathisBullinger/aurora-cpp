#pragma once

#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono;

class Engine {
  public:
    ~Engine();
  
    void start();

    Window* openWindow();
    Window* closeWindow(Window* window);
  
  private:
    void run();
    void processEvents();
    void handleEvent(SDL_Event& event);
  
    std::vector<Window*> windows;
    bool running = false;
};
