#include "./inputReceiver.hpp"

namespace aur {

std::set<InputReceiver*> InputReceiver::active;

InputReceiver::~InputReceiver() {
  stop();
}

void InputReceiver::start() {
  active.insert(this);
}

void InputReceiver::stop() {
  active.erase(this);
}

}
