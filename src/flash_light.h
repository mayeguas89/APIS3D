#pragma once

#include "light.h"

class FlashLight: public Light
{
public:
  FlashLight(const glm::vec3& color,
             float ambient_contribution,
             float difuse_contribution,
             float specular_contribution,
             float light_range,
             const glm::vec3& position,
             const glm::vec3& direction,
             float cutoff_angle_degrees,
             float outer_cutoff_angle_degrees);
  void Update(float delta_time) override;
};
