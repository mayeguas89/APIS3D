#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "input_manager.h"

#include <unordered_map>

class GLFWInputManager: public InputManager
{
public:
  GLFWInputManager();

  void Init() override;

  bool IsPressed(char key) override;

  static void KeyManager(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  static std::unordered_map<int, bool> keyState_;
};
