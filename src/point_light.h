#pragma once

#include "light.h"

class PointLight: public Light
{
public:
  PointLight(const glm::vec3& color,
             float ambient_contribution,
             float difuse_contribution,
             float specular_contribution,
             float light_range,
             const glm::vec3& position);

  void Update(float delta_time) override;
};
