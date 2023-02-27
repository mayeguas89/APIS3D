#include "directional_light.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color):
  Light(Type::kDirectional, glm::vec3(0.f, 0.f, 0.f), direction, color, 0.f)
{}

void DirectionalLight::Update(float delta_time)
{
  Light::Update(delta_time);
}
