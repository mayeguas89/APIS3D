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
const glm::vec3& Material::GetColor() const
{
  return color_;
}
uint8_t Material::GetShininess() const
{
  return shininess_;
}
void Material::SetColor(const glm::vec3& color)
{
  color_ = color;
}
void Material::SetShininess(uint8_t shininess)
{
  shininess_ = shininess;
}
