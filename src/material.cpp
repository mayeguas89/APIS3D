#include "material.h"

Material::Material()
{
  render_program_ = nullptr;
  texture_ = nullptr;
}

Material::~Material()
{
  if (render_program_)
    delete render_program_;
  if (texture_)
    delete texture_;
}

Texture* Material::GetTexture()
{
  return texture_;
}

void Material::SetTexture(Texture* texture)
{
  texture_ = texture;
}
