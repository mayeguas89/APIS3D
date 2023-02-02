#pragma once

#include "common.h"
#include "object.h"
#include "render.h"

class GL1Render: public Render
{
public:
  GL1Render(int width, int height);
  ~GL1Render();

  void Init() override;
  void SetupObject(Object* object) override;
  void RemoveObject(Object* object) override;
  void DrawObjects(const std::vector<Object*>* objects) override;
  bool IsClosed() override;
  void SetKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods));
  void Clear() override;
  void SwapBuffers() override;

private:
  GLFWwindow* window_;
};
