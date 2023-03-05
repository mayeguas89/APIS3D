#include "texture.h"

Texture::Texture(): id_{0} {}

unsigned int Texture::GetTextureId()
{
  return id_;
}

Texture::Type Texture::GetType()
{
  return type_;
}

void Texture::SetType(Texture::Type type)
{
  type_ = type;
}

const glm::vec2& Texture::GetSize()
{
  return size_;
}
