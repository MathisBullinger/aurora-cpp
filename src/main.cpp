#include <iostream>
#include <SDL2/SDL.h>
#include "engine.hpp"

int main() {
  Engine engine;
  engine.openWindow();
  engine.start();
  return 0;
}
