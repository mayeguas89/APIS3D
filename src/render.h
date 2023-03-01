#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "object.h"

class Render
{
public:
  Render(int width, int height);
  virtual ~Render() = default;

  int GetWidth();
  int GetHeight();

  virtual void SetSize(int width, int height);

  virtual void Init() = 0;
  virtual void SetupObject(Object* object) = 0;
  virtual void RemoveObject(Object* object) = 0;
  virtual void DrawObjects(const std::vector<Object*>* objects) = 0;
  virtual void DrawObject(Object* object) = 0;
  virtual bool IsClosed() = 0;
  virtual void Clear() = 0;
  virtual void SwapBuffers() = 0;

protected:
  int width_;
  int height_;
};
