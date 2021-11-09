#pragma once

#include "input/input.hpp"
#include <set>

namespace aur {

class InputReceiver {
public:
  virtual ~InputReceiver();
  virtual void update(Input& input) = 0;

  void start();
  void stop();

  static std::set<InputReceiver*> active;
};

}
