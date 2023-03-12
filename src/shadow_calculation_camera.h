#pragma once

#include "camera.h"
#include "light.h"
class ShadowCalculationCamera: public Camera
{
public:
  ShadowCalculationCamera(Light* light);
  virtual void ComputeProjectionMatrix(ProjectionType type) override;
  void Update(float delta_time) override;

private:
  virtual void ComputeViewMatrix() override;
  Light* light_;
};