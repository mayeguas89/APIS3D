#pragma once
#include "point_light.h"

class OrbitalLight: public PointLight
{
public:
  OrbitalLight(const glm::vec3& color,
               float ambient_contribution,
               float difuse_contribution,
               float specular_contribution,
               float light_range,
               const glm::vec3& position,
               float orbitation_speed = glm::quarter_pi<float>());
  void Update(float delta_time) override;

private:
  // Un cuarto de vuelta por segundo
  float orbitation_speed_;
};