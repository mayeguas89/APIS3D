#include "point_light.h"

PointLight::PointLight(const glm::vec3& color,
                       float ambient_contribution,
                       float difuse_contribution,
                       float specular_contribution,
                       float light_range,
                       const glm::vec3& position):
  Light(color, ambient_contribution, difuse_contribution, specular_contribution)
{
  light_type_ = Type::kPoint;
  SetLightRange(light_range);
  SetPosition(glm::vec4{position, 1.f});
}

void PointLight::Update(float delta_time)
{
  Light::Update(delta_time);
}
