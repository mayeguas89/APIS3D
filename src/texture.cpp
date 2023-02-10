#include "texture.h"

Texture::Texture(): id_{0} {}

unsigned int Texture::GetTextureId()
{
  return id_;
}

glm::vec2 Texture::GetSize()
{
  return size_;
}
