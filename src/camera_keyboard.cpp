#include "camera_keyboard.h"

#include "system.h"

CameraKeyboard::CameraKeyboard(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up):
  Camera(type, position, look_at, up)
{
  angle_ = glm::radians((float)System::GetInputManager()->GetYScroll());
  ComputeProjectionMatrix(type);
}

void CameraKeyboard::Update(float delta_time)
{
  if (System::GetInputManager()->IsPressed(GLFW_MOUSE_BUTTON_2))
  {
    auto last_y_position = position_.y;
    if (System::GetInputManager()->IsPressed('D'))
      position_ += glm::vec4(delta_time * camera_speed_ * glm::normalize(glm::cross(look_at_, up_)), 1.f);
    if (System::GetInputManager()->IsPressed('A'))
      position_ -= glm::vec4(delta_time * camera_speed_ * glm::normalize(glm::cross(look_at_, up_)), 1.f);
    if (System::GetInputManager()->IsPressed('W'))
      position_ += glm::vec4(delta_time * camera_speed_ * look_at_, 1.f);
    if (System::GetInputManager()->IsPressed('S'))
      position_ -= glm::vec4(delta_time * camera_speed_ * look_at_, 1.f);
    if (System::GetInputManager()->IsPressed('E'))
      last_y_position -= delta_time * camera_speed_;
    if (System::GetInputManager()->IsPressed('Q'))
      last_y_position += delta_time * camera_speed_;

    position_.y = last_y_position; // keeps the user at the ground level (xz plane)

    look_at_ = glm::normalize(System::GetInputManager()->GetMouseRotationDirection());
    angle_ = (float)(glm::radians(System::GetInputManager()->GetYScroll()));
  }
  ComputeProjectionMatrix(projection_type_);
  ComputeViewMatrix();
}

void CameraKeyboard::ComputeProjectionMatrix(ProjectionType type)
{
  if (type == ProjectionType::Perspective)
    projection_mtx_ =
      glm::perspective(angle_, System::GetAspectRatio(), System::GetNearPlane(), System::GetFarPlane());
  else
    throw std::runtime_error("Projection type is not supported");
}

void CameraKeyboard::ComputeViewMatrix()
{
  view_mtx_ = glm::lookAt(glm::vec3(position_), glm::vec3(position_) + look_at_, up_);
}