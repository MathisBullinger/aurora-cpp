#pragma once

#include "math/vector.hpp"

namespace aur {

class Mouse {
public:
  void move(int x, int y);
  void reset();
  const Vector<2, int>& relativeMovement() const;

private:
  Vector<2, int> frameRelative_;
};

}
