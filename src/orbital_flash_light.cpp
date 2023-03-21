#include "orbital_flash_light.h"

OrbitalFlashLight::OrbitalFlashLight(const glm::vec3& color,
                                     float ambient_contribution,
                                     float difuse_contribution,
                                     float specular_contribution,
                                     float light_range,
                                     const glm::vec3& position,
                                     const glm::vec3& direction,
                                     float cutoff_angle_degrees,
                                     Camera* camera,
                                     float orbitation_speed):
  FlashLight(color,
             ambient_contribution,
             difuse_contribution,
             specular_contribution,
             light_range,
             position,
             direction,
             cutoff_angle_degrees),
  camera_{camera},
  orbitation_speed_{orbitation_speed}
{}

void OrbitalFlashLight::Update(float delta_time)
{
  rotation_.y += delta_time * orbitation_speed_;

  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  model = glm::translate(model, glm::vec3(-position_.x, -position_.y, -position_.z));
  model = glm::rotate(model, rotation_.y, glm::vec3(0.f, 1.f, 0.f));
  model = glm::translate(model, glm::vec3(position_.x, position_.y, position_.z));

  model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  if (camera_)
    SetDirection(camera_->GetLookAt());

  cube_->SetModelMatrix(glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)));
}
