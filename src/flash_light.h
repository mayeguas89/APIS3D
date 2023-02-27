#pragma once

#include "light.h"

class FlashLight: public Light
{
public:
  FlashLight(const glm::vec3& position,
             const glm::vec3& direction,
             const glm::vec3& color,
             float cut_angle_degrees);
  void Update(float delta_time) override;
};
