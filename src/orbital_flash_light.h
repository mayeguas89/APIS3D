#pragma once
#include "camera.h"
#include "flash_light.h"

class OrbitalFlashLight: public FlashLight
{
public:
  OrbitalFlashLight(const glm::vec3& color,
                    float ambient_contribution,
                    float difuse_contribution,
                    float specular_contribution,
                    float light_range,
                    const glm::vec3& position,
                    const glm::vec3& direction,
                    float cutoff_angle_degrees,
                    float outer_cutoff_angle_degrees,
                    Camera* camera,
                    float orbitation_speed = glm::quarter_pi<float>());
  void Update(float delta_time) override;

private:
  // Un cuarto de vuelta por segundo
  float orbitation_speed_;
  Camera* camera_;
};