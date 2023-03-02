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

  void SetupParticle(Emitter* emitter) override;
  void SetupObject(Object* object) override;
  void RemoveObject(Object* object) override;
  void DrawObjects(const std::vector<Object*>* objects) override;
  void DrawObject(Object* object) override;
  void DrawParticles(Emitter* emitter) override;
  bool IsClosed() override;
  void SetSize(int width, int height) override;

  void SetKeyCallback(void (*callback)(GLFWwindow* window, int key, int scancode, int action, int mods));
  void SetCursorPositionCallback(void (*callback)(GLFWwindow* window, double x_key, double y_pos));
  void SetMouseButtonCallback(void (*callback)(GLFWwindow* window, int button, int action, int mods));
  void SetScrollCallback(void (*callback)(GLFWwindow* window, double x_offset, double y_offset));
  void Clear() override;
  void SwapBuffers() override;

protected:
  GLFWwindow* window_;
  glm::vec4 clear_color_;
};
