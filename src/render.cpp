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

void Render::SetSize(int width, int height)
{
  width_ = width;
  height_ = height;
}
