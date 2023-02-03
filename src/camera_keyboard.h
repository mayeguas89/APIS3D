#pragma once

#include "camera.h"

class CameraKeyboard: public Camera
{
public:
  CameraKeyboard(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up);
  void Update(float delta_time) override;

protected:
  void ComputeProjectionMatrix(ProjectionType type) override;
  void ComputeViewMatrix() override;
};