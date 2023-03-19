#include "free_camera.h"

#include "system.h"

FreeCamera::FreeCamera(glm::vec3 position, glm::vec3 look_at, glm::vec3 up, float camera_speed):
  Camera(Camera::ProjectionType::Perspective, position, look_at, up)
{
  camera_speed_ = camera_speed;
  angle_ = glm::radians((float)System::GetInputManager()->GetYScroll());
  ComputeProjectionMatrix(projection_type_);
}

void FreeCamera::Update(float delta_time)
{
  static bool first_mouse = true;
  static double old_pos_x = 0.0;
  static double old_pos_y = 0.0;
  static double pos_x = 0.0;
  static double pos_y = 0.0;
  static const double kMouseSensibility = 0.01;

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

    old_pos_x = pos_x;
    old_pos_y = pos_y;

    pos_x = System::GetInputManager()->GetMouseXPos();
    pos_y = System::GetInputManager()->GetMouseYPos();

    auto yaw = (float)((old_pos_x - pos_x) * kMouseSensibility);
    auto pitch = (float)((pos_y - old_pos_y) * kMouseSensibility);

    auto right = glm::cross(glm::normalize(glm::vec3(position_) - look_at_), glm::normalize(up_));
    auto rot_pitch = glm::rotate(glm::mat4{1.0f}, pitch, right);
    auto rot_yaw = glm::rotate(glm::mat4{1.0f}, yaw, up_);
    auto rot = rot_pitch * rot_yaw;

    if (first_mouse)
    {
      first_mouse = false;
    }
    else
    {
      look_at_ = glm::vec3{glm::normalize(rot * glm::vec4(look_at_, 0.f))};
    }
    angle_ = (float)(glm::radians(System::GetInputManager()->GetYScroll()));
  }
  else
  {
    first_mouse = true;
  }
  ComputeProjectionMatrix(projection_type_);
  ComputeViewMatrix();
}

void FreeCamera::ComputeProjectionMatrix(ProjectionType type)
{
  if (type == ProjectionType::Perspective)
    projection_mtx_ =
      glm::perspective(angle_, System::GetAspectRatio(), System::GetNearPlane(), System::GetFarPlane());
  else
    throw std::runtime_error("Projection type is not supported");
}

void FreeCamera::ComputeViewMatrix()
{
  view_mtx_ = glm::lookAt(glm::vec3(position_), glm::vec3(position_) + look_at_, up_);
}