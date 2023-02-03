#include "camera.h"

Camera::Camera(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up):
  Entity(),
  look_at_{look_at},
  up_{up},
  angle_{glm::half_pi<float>()}
{
  position_ = glm::vec4(position, 1.f);
  radius_ = glm::vec3(look_at_ - glm::vec3(position_));
}

const glm::mat4& Camera::GetViewMatrix()
{
  return view_mtx_;
}

const glm::mat4& Camera::GetProjectionMatrix()
{
  return projection_mtx_;
}

void Camera::SetSpeed(float speed)
{
  camera_speed_ = speed;
}
