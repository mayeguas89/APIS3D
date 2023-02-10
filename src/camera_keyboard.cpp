#include "camera_keyboard.h"

#include "system.h"

CameraKeyboard::CameraKeyboard(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up):
  Camera(type, position, look_at, up)
{
  ComputeProjectionMatrix(type);
}

void CameraKeyboard::Update(float delta_time)
{
  if (System::GetInputManager()->IsPressed(GLFW_MOUSE_BUTTON_2))
  {
    if (System::GetInputManager()->IsPressed('D'))
      position_.x -= delta_time * camera_speed_;
    if (System::GetInputManager()->IsPressed('A'))
      position_.x += delta_time * camera_speed_;

    static double prev_x, prev_y;

    double speed_my = 0;

    speed_my = prev_y - System::GetInputManager()->GetMouseYPos();
    prev_y = System::GetInputManager()->GetMouseYPos();
    rotation_.x += static_cast<float>(speed_my) * camera_speed_;

    look_at_ = glm::rotate(glm::mat4(1.f), rotation_.x, glm::vec3(1.f, 0.f, 0.f)) * glm::vec4(radius_, 1.f);
    look_at_ += glm::vec3(position_);
  }
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
  view_mtx_ = glm::lookAt(glm::vec3(position_), look_at_, up_);
}