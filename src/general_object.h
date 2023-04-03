#pragma once

#include "object3d.h"

class GeneralObject: public Object3D
{
public:
  GeneralObject() {}

  void Update(float delta_time) override
  {
    Object3D::Update(delta_time);
    Entity::ComputeModelMatrix();
  }
};