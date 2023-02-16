#pragma once
#define GLAD_ONLY_HEADER
#include "common.h"
class InputManager
{
public:
  virtual ~InputManager() = default;

  virtual void Init() = 0;
  virtual bool IsPressed(char key) = 0;
  virtual double GetMouseXPos() = 0;
  virtual double GetMouseYPos() = 0;
  virtual glm::vec3 GetMouseRotationDirection() = 0;
  virtual double GetYScroll() = 0;
};
