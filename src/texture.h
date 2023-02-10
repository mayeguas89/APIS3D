#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"

class Texture
{
public:
  Texture();
  virtual ~Texture() = default;
  virtual bool Load(const std::string& filename) = 0;
  virtual void Bind() = 0;
  unsigned int GetTextureId();
  glm::vec2 GetSize();

protected:
  unsigned int id_ = 0;
  glm::vec2 size_;
};