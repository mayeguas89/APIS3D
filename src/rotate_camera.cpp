#include "rotate_camera.h"

#include "system.h"

RotateCamera::RotateCamera(ProjectionType type,
                           glm::vec3 position,
                           glm::vec3 look_at,
                           glm::vec3 up,
                           float rotation_rate):
  Camera(type, position, look_at, up)
{
  camera_speed_ = rotation_rate;
  angle_ = glm::radians((float)System::GetInputManager()->GetYScroll());
  ComputeProjectionMatrix(type);
}

void RotateCamera::Update(float delta_time)
{
  auto cam_focus_vector = glm::vec3(position_) - look_at_;
  auto t_matrix = glm::translate(glm::mat4(1.f), -cam_focus_vector);
  t_matrix = glm::rotate(t_matrix, delta_time * camera_speed_, glm::vec3{0.f, 1.f, 0.f});
  t_matrix = glm::translate(t_matrix, cam_focus_vector);

  cam_focus_vector = glm::vec3(t_matrix * glm::vec4(cam_focus_vector, 1.f));

  position_ = glm::vec4(cam_focus_vector + look_at_, 1.f);

  angle_ = (float)(glm::radians(System::GetInputManager()->GetYScroll()));
  ComputeProjectionMatrix(projection_type_);
  ComputeViewMatrix();
}

void RotateCamera::ComputeProjectionMatrix(ProjectionType type)
{
  if (type == ProjectionType::Perspective)
    projection_mtx_ =
      glm::perspective(angle_, System::GetAspectRatio(), System::GetNearPlane(), System::GetFarPlane());
  else
    throw std::runtime_error("Projection type is not supported");
}

void RotateCamera::ComputeViewMatrix()
{
  view_mtx_ = glm::lookAt(glm::vec3(position_), look_at_, up_);
}