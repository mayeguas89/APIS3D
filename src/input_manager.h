#pragma once

class InputManager
{
public:
  virtual ~InputManager() = default;

  virtual void Init() = 0;
  virtual bool IsPressed(char key) = 0;
  virtual double GetMouseXPos() = 0;
  virtual double GetMouseYPos() = 0;
};
