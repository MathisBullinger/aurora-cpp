#include "app.hpp"
#include "input/inputReceiver.hpp"

namespace aur {

void App::start() {
  run();
}

App::~App() {
  for (auto window : windows) delete window;
  windows.clear();
}

void App::run() {
  running = true;
  auto frametime = duration_cast<microseconds>(seconds(1)) / 60;

  Renderer renderer;

  auto tLast = high_resolution_clock::now();
  while (running) {
    auto t0 = high_resolution_clock::now();
    auto ftd = duration_cast<microseconds>(t0 - tLast).count() / 1e6;
    tLast = t0;
    
    processEvents();

    for (auto window : windows) {
      for (const auto& receiver : InputReceiver::active)
        receiver->update(input);
      
      renderer.render(ftd);
      window->swap();
    }

    auto td = duration_cast<microseconds>(high_resolution_clock::now() - t0) - frametime;
    if (td.count() > 0) std::this_thread::sleep_for(td);
  }
}

void App::processEvents() {
  input.getMouse().reset();
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) handleEvent(event);
}

void App::handleEvent(SDL_Event& event) {
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

    case SDL_MOUSEMOTION:
      input.getMouse().move(event.motion.xrel, event.motion.yrel);
      break;
  }
}

Window* App::openWindow() {
  Window* window = new Window();
  windows.push_back(window);
  return window;
}

Window* App::closeWindow(Window* window) {
  auto it = std::find(windows.begin(), windows.end(), window);
  if (it == windows.end()) return window;
  windows.erase(it);
  delete window;
  return nullptr;
}

}
