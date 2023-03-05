#pragma once

#include "object3d.h"

class CubeMap: public Object3D
{
public:
  CubeMap() {}
  virtual void Update(float delta_time) override
  {
    ComputeModelMatrix();
  }
};
