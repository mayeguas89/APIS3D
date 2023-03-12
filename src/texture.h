#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"

class Texture
{
public:
  enum class Type : int
  {
    kColor2D = 0,
    kCubeMap = 1,
    kNormal = 2,
    kDepthFb = 3,
    kColorFb = 4,
    kNone
  };

  Texture();
  virtual ~Texture() = default;
  virtual bool Load(const std::vector<std::string>& filenames, Type type = Type::kColor2D) = 0;
  virtual void Bind(unsigned int index = 0) = 0;
  unsigned int GetTextureId();
  Type GetType();
  void SetType(Type type);
  const glm::vec2& GetSize();

protected:
  unsigned int id_ = 0;
  glm::vec2 size_ = glm::vec2{0.f};
  Type type_ = Type::kNone;
};