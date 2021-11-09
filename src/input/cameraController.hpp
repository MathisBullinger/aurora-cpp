#pragma once

#include "input/inputReceiver.hpp"
#include "renderer/camera.hpp"

namespace aur {

class CameraController : public InputReceiver {
public:
  CameraController(Camera& camera);
  void update(Input& input);

private:
  Camera& camera_;
};
  
}
