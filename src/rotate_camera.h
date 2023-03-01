#pragma once

#include "camera.h"

class RotateCamera: public Camera
{
public:
  RotateCamera(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float rotation_rate);
  void Update(float delta_time) override;
  void ComputeProjectionMatrix(ProjectionType type) override;

protected:
  void ComputeViewMatrix() override;
};