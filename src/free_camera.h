#pragma once

#include "camera.h"

class FreeCamera: public Camera
{
public:
  FreeCamera(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float camera_speed);
  void Update(float delta_time) override;
  void ComputeProjectionMatrix(ProjectionType type) override;

protected:
  void ComputeViewMatrix() override;
};