#include "glfw_input_manager.h"

std::unordered_map<int, bool> GLFWInputManager::keyState_;
double GLFWInputManager::pos_x_ = 0.;
double GLFWInputManager::pos_y_ = 0.;
double GLFWInputManager::old_pos_x_ = 0.;
double GLFWInputManager::old_pos_y_ = 0.;
double GLFWInputManager::scroll_y_ = 0.;
double GLFWInputManager::scroll_x_ = 0.;

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
      std::cout << "Evento de press de tecla\n";
      keyState_[key] = true;
      break;
    }
    case GLFW_RELEASE:
    {
      std::cout << "Evento de release de tecla\n";
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

  std::cout << "Mouse position (" << x_pos << ", " << y_pos << ")" << std::endl;
}

void GLFWInputManager::ScrollManager(GLFWwindow* window, double x_offset, double y_offset)
{
  scroll_x_ = x_offset;
  scroll_y_ = y_offset;
  std::cout << "Mouse scroll (" << x_offset << ", " << y_offset << ")" << std::endl;
}

void GLFWInputManager::MouseKeyManager(GLFWwindow* window, int button, int action, int mods)
{
  switch (action)
  {
    case GLFW_PRESS:
      std::cout << "Evento de press de tecla de raton\n";
      keyState_[button] = true;
      break;
    case GLFW_RELEASE:
      std::cout << "Evento de release de tecla de raton\n";
      keyState_[button] = false;
      break;
    default:
      break;
  }
}