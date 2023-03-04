#include "directional_light.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction,
                                   const glm::vec3& color,
                                   float ambient_contribution,
                                   float difuse_contribution,
                                   float specular_contribution):
  Light(Type::kDirectional,
        glm::vec3(0.f, 0.f, 0.f),
        direction,
        color,
        0.f,
        1.f,
        ambient_contribution,
        difuse_contribution,
        specular_contribution)
{}

void DirectionalLight::Update(float delta_time)
{
  Light::Update(delta_time);
}
