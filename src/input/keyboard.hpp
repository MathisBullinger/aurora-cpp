#pragma once

#include <SDL2/SDL.h>
#include <set>

namespace aur {

class Keyboard {
public:
  void press(const SDL_Keycode key);
  void release(const SDL_Keycode key);
  bool isPressed(const SDL_Keycode key) const;

private:
  std::set<SDL_Keycode> pressed_;
};
  
}
