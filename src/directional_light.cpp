#include "directional_light.h"

DirectionalLight::DirectionalLight(const glm::vec3& color,
                                   float ambient_contribution,
                                   float difuse_contribution,
                                   float specular_contribution,
                                   const glm::vec3& direction):
  Light(color, ambient_contribution, difuse_contribution, specular_contribution)
{
  light_type_ = Type::kDirectional;
  SetDirection(direction);
}

void DirectionalLight::Update(float delta_time)
{
  Light::Update(delta_time);
}