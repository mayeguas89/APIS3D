#include "render.h"

Render::Render(int width, int height): width_{width}, height_{height} {}

int Render::GetWidth()
{
  return width_;
}

int Render::GetHeight()
{
  return height_;
}

const glm::vec4& Render::GetClearColor()
{
  return clear_color_;
}

void Render::SetClearColor(const glm::vec4& color)
{
  clear_color_ = color;
}

void Render::SetSize(int width, int height)
{
  width_ = width;
  height_ = height;
}
