#pragma once
#include "camera.h"
class MirrorCamera: public Camera
{
public:
  MirrorCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up);
  virtual void Update(float delta_time) override;
  virtual void ComputeProjectionMatrix(ProjectionType type) override;
  virtual void ComputeViewMatrix() override;
};
