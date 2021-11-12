#include "./cameraController.hpp"

namespace aur {

CameraController::CameraController(FPSCamera& camera)
  : camera_{camera} {};

void CameraController::update(Input& input) {
  auto key = input.getKeyboard();
  
  float v = .1;
  auto rv = 2_deg;
  
  if (key.isPressed(SDLK_w))
    camera_.move(camera_.getDirView() * v);

  if (key.isPressed(SDLK_s))
    camera_.move(-camera_.getDirView() * v);

  if (key.isPressed(SDLK_a))
    camera_.move(camera_.getDirRight() * v);

  if (key.isPressed(SDLK_d))
    camera_.move(camera_.getDirRight() * -v);

  if (key.isPressed(SDLK_LSHIFT))
    camera_.move(camera_.getDirUp() * v);

  if (key.isPressed(SDLK_LCTRL))
    camera_.move(camera_.getDirUp() * -v);

  auto [x, y] = input.getMouse().relativeMovement();
  if (abs(x)) camera_.addYaw(angle::degrees(x) / 5);
  if (abs(y)) camera_.addPitch(angle::degrees(y) / 5);
};

}
