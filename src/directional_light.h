#pragma once
#include "light.h"

class DirectionalLight: public Light
{
public:
  DirectionalLight(const glm::vec3& direction, const glm::vec3& color);
  void Update(float delta_time) override;
};
