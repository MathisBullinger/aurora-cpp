#pragma once

#include "input/inputReceiver.hpp"
#include "renderer/camera.hpp"

namespace aur {

class CameraController : public InputReceiver {
public:
  CameraController(FPSCamera& camera);
  void update(Input& input);

private:
  FPSCamera& camera_;
};
  
}
