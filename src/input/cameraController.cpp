#include "./cameraController.hpp"
#include <iostream>

namespace aur {

FreeCameraController* CameraController::create(FreeCamera& camera) {
  return new FreeCameraController(camera);
}

FPSCameraController* CameraController::create(FPSCamera& camera) {
  return new FPSCameraController(camera);
}

FreeCameraController::FreeCameraController(FreeCamera& camera)
  : CameraController{camera}, camera_{camera} {}

FPSCameraController::FPSCameraController(FPSCamera& camera)
  : CameraController{camera}, camera_{camera} {}

void CameraController::update(Input& input) {
  auto key = input.getKeyboard();
  
  float v = .1;
  auto rv = 2_deg;
  
  if (key.isPressed(SDLK_w))
    camera_.move(camera_.getDirView() * v);

  if (key.isPressed(SDLK_s))
    camera_.move(-camera_.getDirView() * v);

  if (key.isPressed(SDLK_a))
    camera_.move(-camera_.getDirRight() * v);

  if (key.isPressed(SDLK_d))
    camera_.move(camera_.getDirRight() * v);

  if (key.isPressed(SDLK_LSHIFT))
    camera_.move(camera_.getDirUp() * v);

  if (key.isPressed(SDLK_LCTRL))
    camera_.move(camera_.getDirUp() * -v);
};

void FPSCameraController::update(Input& input) {
  CameraController::update(input);
  auto key = input.getKeyboard();

  if (key.isPressed(SDLK_e))
    camera_.roll(angle::degrees(-1));

  if (key.isPressed(SDLK_q))
    camera_.roll(angle::degrees(1));

  auto [x, y] = input.getMouse().relativeMovement();
  if (abs(x)) camera_.addYaw(-angle::degrees(x) / 5);
  if (abs(y)) camera_.addPitch(-angle::degrees(y) / 5);
}

}
