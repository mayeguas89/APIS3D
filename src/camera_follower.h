#pragma once
#include "camera.h"
#include "object3d.h"

#include <memory>
class CameraFollower: public Object3D
{
public:
  CameraFollower(std::shared_ptr<Camera> camera);
  virtual void Update(float delta_time) override;

private:
  std::shared_ptr<Camera> camera_;
};