#include "glfw_input_manager.h"

std::unordered_map<int, bool> GLFWInputManager::keyState_;

GLFWInputManager::GLFWInputManager() {}

void GLFWInputManager::Init()
{
  for (int i = 0; i < GLFW_KEY_LAST; i++)
  {
    keyState_[i] = false;
  }
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
