#include "engine.hpp"
#include "input/inputReceiver.hpp"

namespace aur {

void Engine::start() {
  run();
}

Engine::~Engine() {
  for (auto window : windows) delete window;
  windows.clear();
}

void Engine::run() {
  running = true;
  auto frametime = duration_cast<microseconds>(seconds(1)) / 60;

  Renderer renderer;

  while (running) {
    auto t0 = high_resolution_clock::now();
    
    processEvents();

    for (auto window : windows) {
      window->clear();

      for (const auto& receiver : InputReceiver::active)
        receiver->update(input);
      
      renderer.render();
      window->swap();
    }

    auto td = duration_cast<microseconds>(high_resolution_clock::now() - t0) - frametime;
    if (td.count() > 0) std::this_thread::sleep_for(td);
  }
}

void Engine::processEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) handleEvent(event);
}

void Engine::handleEvent(SDL_Event& event) {
  switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;

    case SDL_KEYDOWN:
      input.getKeyboard().press(event.key.keysym.sym);
      break;
    
    case SDL_KEYUP:
      input.getKeyboard().release(event.key.keysym.sym);
      break;
  }
}

Window* Engine::openWindow() {
  Window* window = new Window();
  windows.push_back(window);
  return window;
}

Window* Engine::closeWindow(Window* window) {
  auto it = std::find(windows.begin(), windows.end(), window);
  if (it == windows.end()) return window;
  windows.erase(it);
  delete window;
  return nullptr;
}

}
