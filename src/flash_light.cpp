#include "flash_light.h"
FlashLight::FlashLight(const glm::vec3& position,
                       const glm::vec3& direction,
                       const glm::vec3& color,
                       float cut_angle_degrees):
  Light(Light::Type::kFocal, position, direction, color, cut_angle_degrees)
{
  linear_attenuation_ = 1.f;
}

void FlashLight::Update(float delta_time)
{
  Light::Update(delta_time);
}
