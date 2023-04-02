#include "flash_light.h"

FlashLight::FlashLight(const glm::vec3& color,
                       float ambient_contribution,
                       float difuse_contribution,
                       float specular_contribution,
                       float light_range,
                       const glm::vec3& position,
                       const glm::vec3& direction,
                       float cutoff_angle_degrees,
                       float outer_cutoff_angle_degrees):
  Light(color, ambient_contribution, difuse_contribution, specular_contribution)
{
  light_type_ = Type::kFocal;
  SetLightRange(light_range);
  SetPosition(glm::vec4{position, 1.f});
  SetDirection(direction);
  SetCutOff(cutoff_angle_degrees);
  SetOuterCutOff(outer_cutoff_angle_degrees);
}

void FlashLight::Update(float delta_time)
{
  Light::Update(delta_time);
}