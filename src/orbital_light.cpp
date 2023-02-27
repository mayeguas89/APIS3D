//
// Created by mayeg on 25/02/2023.
//

#include "orbital_light.h"
OrbitalLight::OrbitalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color):
  Light(Light::Type::kPoint, position, direction, color, 0.f)
{}

void OrbitalLight::Update(float delta_time)
{
  rotation_.y += delta_time * kOrbitationSpeed;

  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  model = glm::translate(model, glm::vec3(-position_.x, -position_.y, -position_.z));
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  model = glm::translate(model, glm::vec3(position_.x, position_.y, position_.z));

  model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  cube_->SetModelMatrix(glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)));
}
