#pragma once

#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "input/input.hpp"

using namespace std::chrono;

namespace aur {

class App {
public:
  ~App();
  
  void start();

  Window* openWindow();
  Window* closeWindow(Window* window);
  
private:
  void run();
  void processEvents();
  void handleEvent(SDL_Event& event);
  
  bool running = false;
  std::vector<Window*> windows;
  Input input;
};

}
