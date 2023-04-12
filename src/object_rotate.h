#pragma once

#include "object3d.h"

class ObjectRotate: public Object3D
{
public:
  ObjectRotate() {}

  void Update(float delta_time) override
  {
    rotation_.z += delta_time * glm::quarter_pi<float>();
    Object3D::Update(delta_time);
    Entity::ComputeModelMatrix();
  }
};