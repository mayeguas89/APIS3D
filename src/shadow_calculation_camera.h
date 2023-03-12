#pragma once

#include "camera.h"

class ShadowCalculationCamera: public Camera
{
public:
  ShadowCalculationCamera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);
  virtual void ComputeProjectionMatrix(ProjectionType type) override;
  void Update(float delta_time) override;

private:
  virtual void ComputeViewMatrix() override;
};