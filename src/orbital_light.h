#pragma once
#include "light.h"

class OrbitalLight: public Light
{
public:
  OrbitalLight(const glm::vec3& position,
               const glm::vec3& direction,
               const glm::vec3& color,
               float linear_attenuation,
               float ambient_contribution,
               float difuse_contribution,
               float specular_contribution);
  void Update(float delta_time) override;

private:
  // Un cuarto de vuelta por segundo
  inline static float kOrbitationSpeed = glm::quarter_pi<float>();
};