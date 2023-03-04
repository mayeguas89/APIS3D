#pragma once
#include "light.h"

class DirectionalLight: public Light
{
public:
  DirectionalLight(const glm::vec3& direction,
                   const glm::vec3& color,
                   float ambient_contribution,
                   float difuse_contribution,
                   float specular_contribution);
  void Update(float delta_time) override;
};
