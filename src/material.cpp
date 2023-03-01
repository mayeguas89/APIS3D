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

bool Material::IsLightEnabled()
{
  return light_enabled_;
}
bool Material::IsCullingEnabled()
{
  return culling_enabled_;
}
bool Material::IsDepthTestEnabled()
{
  return depth_test_enabled_;
}
void Material::SetLightEnabled(bool value)
{
  light_enabled_ = value;
}
void Material::SetCullingEnabled(bool value)
{
  culling_enabled_ = value;
}
void Material::SetDepthTestEnabled(bool value)
{
  depth_test_enabled_ = value;
}
Material::BlendMode Material::GetBlendMode()
{
  return blend_mode_;
}
void Material::SetBlendMode(BlendMode value)
{
  blend_mode_ = value;
}

void Material::SetAlpha(float alpha)
{
  alpha_ = alpha;
}

float Material::GetAlpha()
{
  return alpha_;
}
