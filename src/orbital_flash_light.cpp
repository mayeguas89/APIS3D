#include "orbital_flash_light.h"

OrbitalFlashLight::OrbitalFlashLight(const glm::vec3& color,
                                     float ambient_contribution,
                                     float difuse_contribution,
                                     float specular_contribution,
                                     float light_range,
                                     const glm::vec3& position,
                                     const glm::vec3& direction,
                                     float cutoff_angle_degrees,
                                     float outer_cutoff_angle_degrees,
                                     Camera* camera,
                                     float orbitation_speed):
  FlashLight(color,
             ambient_contribution,
             difuse_contribution,
             specular_contribution,
             light_range,
             position,
             direction,
             cutoff_angle_degrees,
             outer_cutoff_angle_degrees),
  camera_{camera},
  orbitation_speed_{orbitation_speed}
{}

void OrbitalFlashLight::Update(float delta_time)
{
  position_ = camera_->GetPosition();
  // static float angle = rotation_.y;
  // angle += delta_time * orbitation_speed_;
  // rotation_.y = angle;

  // glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  // model = glm::translate(model, glm::vec3(-position_.x, -position_.y, -position_.z));
  // model = glm::rotate(model, angle, glm::vec3(0.f, 1.f, 0.f));
  // model = glm::translate(model, glm::vec3(position_.x, position_.y, position_.z));

  // model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  // if (camera_)
  // {
  //   SetDirection(camera_->GetLookAt() - glm::vec3(model_mtx_ * position_));
  // }
  // Light::Update(delta_time);
  // cube_->SetModelMatrix(glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f)));
}
