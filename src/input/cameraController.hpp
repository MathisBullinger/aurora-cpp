#pragma once

#include "input/inputReceiver.hpp"
#include "renderer/camera.hpp"

namespace aur {

class FreeCameraController;
class FPSCameraController;

class CameraController : public InputReceiver {
public:
  static FreeCameraController* create(FreeCamera& camera);
  static FPSCameraController* create(FPSCamera& camera);
  virtual void update(Input& input);

protected:
  CameraController(Camera& camera): camera_{camera} {};
  Camera& camera_;
};

class FreeCameraController : public CameraController {
public:
  FreeCameraController(FreeCamera& camera);
  void update(Input& input);
  
private:
  FreeCamera& camera_;
};

class FPSCameraController : public CameraController {
public:
  FPSCameraController(FPSCamera& camera);
  void update(Input& input);

private:
  FPSCamera& camera_;
};

}
