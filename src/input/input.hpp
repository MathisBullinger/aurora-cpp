#pragma once

#include "input/keyboard.hpp"

namespace aur {

class Input {
public:
  Keyboard& getKeyboard() {
    return keyboard_;
  }

private:
  Keyboard keyboard_;
};

}
