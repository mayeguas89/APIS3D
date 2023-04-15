//
// Created by mayeg on 25/02/2023.
//

#include "orbital_light.h"

#include "system.h"

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
  static float angle = rotation_.y;
  static glm::vec4 old_position = position_;
  angle += delta_time * orbitation_speed_;
  rotation_.y = angle;

  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(old_position));

  model = glm::translate(model, glm::vec3(-old_position.x, -old_position.y, -old_position.z));
  model = glm::rotate(model, angle, glm::vec3(0.0f, 1.f, 0.f));
  model = glm::translate(model, glm::vec3(old_position.x, old_position.y, old_position.z));
  model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.f, 0.f));

  model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  cube_->SetModelMatrix(glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)));

  position_ = model_mtx_ * old_position;
}