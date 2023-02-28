#include "flash_light.h"
FlashLight::FlashLight(const glm::vec3& position,
                       const glm::vec3& direction,
                       const glm::vec3& color,
                       float cutoff_angle_degrees,
                       float linear_attenuation,
                       float ambient_contribution,
                       float difuse_contribution,
                       float specular_contribution):
  Light(Light::Type::kFocal,
        position,
        direction,
        color,
        cutoff_angle_degrees,
        linear_attenuation,
        ambient_contribution,
        difuse_contribution,
        specular_contribution)
{
  linear_attenuation_ = 1.f;
}

void FlashLight::Update(float delta_time)
{
  Light::Update(delta_time);
}
