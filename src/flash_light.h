#pragma once

#include "light.h"

class FlashLight: public Light
{
public:
  FlashLight(const glm::vec3& position,
             const glm::vec3& direction,
             const glm::vec3& color,
             float cutoff_angle_degrees,
             float linear_attenuation,
             float ambient_contribution,
             float difuse_contribution,
             float specular_contribution);
  void Update(float delta_time) override;
};
