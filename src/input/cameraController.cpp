#include "./cameraController.hpp"

namespace aur {

CameraController::CameraController(Camera& camera)
  : camera_{camera} {};

void CameraController::update(Input& input) {
  auto key = input.getKeyboard();
  
  float v = .1;
  
  if (key.isPressed(SDLK_w))
    camera_.move(camera_.getViewDir() * v);

  if (key.isPressed(SDLK_s))
    camera_.move(-camera_.getViewDir() * v);

  if (key.isPressed(SDLK_d))
    camera_.move(Quaternion{{0,1,0}, -90_deg} * camera_.getViewDir() * v);

  if (key.isPressed(SDLK_a))
    camera_.move(Quaternion{{0,1,0}, 90_deg} * camera_.getViewDir() * v);

  if (key.isPressed(SDLK_LSHIFT))
    camera_.move(Quaternion{{1,0,0}, 90_deg} * camera_.getViewDir() * v);

  if (key.isPressed(SDLK_LCTRL))
    camera_.move(Quaternion{{1,0,0}, -90_deg} * camera_.getViewDir() * v);
};

}
