#include "glfw_input_manager.h"

std::unordered_map<int, bool> GLFWInputManager::keyState_;
double GLFWInputManager::pos_x_ = 0.;
double GLFWInputManager::pos_y_ = 0.;
double GLFWInputManager::old_pos_x_ = 0.;
double GLFWInputManager::old_pos_y_ = 0.;
double GLFWInputManager::scroll_y_ = 0.;
double GLFWInputManager::scroll_x_ = 0.;
double GLFWInputManager::fov_ = 45.;
glm::vec3 GLFWInputManager::rotation_direction_ = glm::vec3(0.f, 0.f, 0.f);

GLFWInputManager::GLFWInputManager() {}

void GLFWInputManager::Init()
{
  for (int i = 0; i < GLFW_KEY_LAST; i++)
  {
    keyState_[i] = false;
  }
}

double GLFWInputManager::GetMouseXPos()
{
  return pos_x_;
}

double GLFWInputManager::GetMouseYPos()
{
  return pos_y_;
}

double GLFWInputManager::GetYScroll()
{
  return fov_;
}

glm::vec3 GLFWInputManager::GetMouseRotationDirection()
{
  return rotation_direction_;
}

bool GLFWInputManager::IsPressed(char key)
{
  return keyState_[(int)key];
}

void GLFWInputManager::KeyManager(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (action)
  {
    case GLFW_PRESS:
    {
      keyState_[key] = true;
      break;
    }
    case GLFW_RELEASE:
    {
      keyState_[key] = false;
      break;
    }
    default:
      break;
  }
}

void GLFWInputManager::MouseManager(GLFWwindow* window, double x_pos, double y_pos)
{
  old_pos_x_ = pos_x_;
  old_pos_y_ = pos_y_;

  pos_x_ = x_pos;
  pos_y_ = y_pos;

  static float yaw = -90.f;
  static float pitch = 0.f;

  yaw += (float)((pos_x_ - old_pos_x_) * kMouseSensibility);
  pitch += (float)((old_pos_y_ - pos_y_) * kMouseSensibility);

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  rotation_direction_.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  rotation_direction_.y = glm::sin(glm::radians(pitch));
  rotation_direction_.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
}

void GLFWInputManager::ScrollManager(GLFWwindow* window, double x_offset, double y_offset)
{
  scroll_x_ = x_offset;
  scroll_y_ = y_offset;
  fov_ -= scroll_y_;
  if (fov_ < 1.f)
    fov_ = 1.f;
  else if (fov_ > 90.f)
    fov_ = 90.f;
}

void GLFWInputManager::MouseKeyManager(GLFWwindow* window, int button, int action, int mods)
{
  switch (action)
  {
    case GLFW_PRESS:
      keyState_[button] = true;
      break;
    case GLFW_RELEASE:
      keyState_[button] = false;
      break;
    default:
      break;
  }
}