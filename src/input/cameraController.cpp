#include "./cameraController.hpp"

namespace aur {

CameraController::CameraController(Camera& camera)
  : camera_{camera} {};

void CameraController::update(Input& input) {
  auto key = input.getKeyboard();
  
  float v = .1;
  auto rv = 2_deg;
  
  if (key.isPressed(SDLK_w))
    camera_.move(camera_.getViewDir() * v);

  if (key.isPressed(SDLK_s))
    camera_.move(-camera_.getViewDir() * v);

  if (key.isPressed(SDLK_a))
    camera_.move(camera_.getUpDir().cross(camera_.getViewDir()) * v);

  if (key.isPressed(SDLK_d))
    camera_.move(camera_.getUpDir().cross(camera_.getViewDir()) * -v);

  if (key.isPressed(SDLK_LSHIFT))
    camera_.move(camera_.getUpDir() * v);

  if (key.isPressed(SDLK_LCTRL))
    camera_.move(camera_.getUpDir() * -v);

  if (key.isPressed(SDLK_LEFT))
    camera_.rotate(Quaternion{camera_.getUpDir(), rv});

  if (key.isPressed(SDLK_RIGHT))
    camera_.rotate(Quaternion{camera_.getUpDir(), -rv});

  if (key.isPressed(SDLK_UP))
    camera_.rotate(Quaternion{camera_.getUpDir().cross(camera_.getViewDir()), -rv});

  if (key.isPressed(SDLK_DOWN))
    camera_.rotate(Quaternion{camera_.getUpDir().cross(camera_.getViewDir()), rv});

  if (key.isPressed(SDLK_e))
    camera_.rotate(Quaternion{camera_.getViewDir(), rv});

  if (key.isPressed(SDLK_q))
    camera_.rotate(Quaternion{camera_.getViewDir(), -rv});

  auto move = input.getMouse().relativeMovement();
  if (move[0] + move[1] == 0) return;

  auto clAng = angle::radians( atan2(move[0], -move[1]) );
  camera_.rotate({ 
    Quaternion{ camera_.getViewDir(), 90_deg + clAng } * camera_.getUpDir(),
    angle::radians(move.magnitude() / 200) 
  });
};

}
