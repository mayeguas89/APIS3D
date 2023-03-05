#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "emitter.h"
#include "light.h"
#include "line.h"
#include "object.h"
class Render
{
public:
  Render(int width, int height);
  virtual ~Render() = default;

  int GetWidth();
  int GetHeight();
  const glm::vec4& GetClearColor();
  void SetClearColor(const glm::vec4& color);

  virtual void SetSize(int width, int height);

  virtual void Init() = 0;
  virtual void SetupObject(Object* object) = 0;
  virtual void SetupParticle(Emitter* emitter) = 0;
  virtual void RemoveObject(Object* object) = 0;
  virtual void SetupLight(Light* light) = 0;
  virtual void SetupAmbient() = 0;
  virtual void DrawObjects(const std::vector<Object*>* objects) = 0;
  virtual void DrawObject(Object* object) = 0;
  virtual void DrawParticles(Emitter* emitter) = 0;
  virtual void DrawLines(const std::vector<Line*>& lines) = 0;
  virtual bool IsClosed() = 0;
  virtual void Clear() = 0;
  virtual void SwapBuffers() = 0;

protected:
  int width_;
  int height_;
  glm::vec4 clear_color_ = {0.2, 0.2, 0.2, 1.0f};
};
