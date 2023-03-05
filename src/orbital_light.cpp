//
// Created by mayeg on 25/02/2023.
//

#include "orbital_light.h"
OrbitalLight::OrbitalLight(const glm::vec3& color,
                           float ambient_contribution,
                           float difuse_contribution,
                           float specular_contribution,
                           float light_range,
                           const glm::vec3& position,
                           float orbitation_speed):
  PointLight(color, ambient_contribution, difuse_contribution, specular_contribution, light_range, position),
  orbitation_speed_{orbitation_speed}
{}

void OrbitalLight::Update(float delta_time)
{
  rotation_.y += delta_time * orbitation_speed_;

  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  model = glm::translate(model, glm::vec3(-position_.x, -position_.y, -position_.z));
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  model = glm::translate(model, glm::vec3(position_.x, position_.y, position_.z));

  model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  cube_->SetModelMatrix(glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)));
}
