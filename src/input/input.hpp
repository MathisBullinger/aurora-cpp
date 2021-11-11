#pragma once

#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace aur {

class Input {
public:
  Keyboard& getKeyboard() {
    return keyboard_;
  }

  Mouse& getMouse() {
    return mouse_;
  }

private:
  Keyboard keyboard_;
  Mouse mouse_;
};

}
