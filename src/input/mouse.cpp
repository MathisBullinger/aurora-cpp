#include "./mouse.hpp"

namespace aur {

void Mouse::move(int x, int y) {
  frameRelative_ += { x, y };
}

void Mouse::reset() {
  frameRelative_ = { 0, 0 };
}

const Vector<2, int>& Mouse::relativeMovement() const {
  return frameRelative_;
}

}
