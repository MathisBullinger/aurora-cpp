#include "./keyboard.hpp"

namespace aur {

void Keyboard::press(const SDL_Keycode key) {
  pressed_.insert(key);
}

void Keyboard::release(const SDL_Keycode key) {
  pressed_.erase(key);
}

bool Keyboard::isPressed(const SDL_Keycode key) const {
  return pressed_.contains(key);
}

}
