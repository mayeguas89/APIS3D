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

  static void MouseManager(GLFWwindow* window, double x_pos, double y_pos);

  static void ScrollManager(GLFWwindow* window, double x_offset, double y_offset);

  static void MouseKeyManager(GLFWwindow* window, int button, int action, int mods);

  double GetMouseXPos() override;
  double GetMouseYPos() override;
  double GetYScroll() override;

  glm::vec3 GetMouseRotationDirection() override;

private:
  inline static const double kMouseSensibility = 0.1;
  static std::unordered_map<int, bool> keyState_;
  static double pos_x_, pos_y_;
  static double old_pos_x_, old_pos_y_;
  static double scroll_y_, scroll_x_;
  static glm::vec3 rotation_direction_;
  static double fov_;
};
