#include "material.h"

Material::Material() {}

Material::~Material()
{
  if (render_program_)
    delete render_program_;
  if (base_texture_)
    delete base_texture_;
  if (normal_texture_)
    delete normal_texture_;
  if (reflection_texture_)
    delete reflection_texture_;
  if (refraction_texture_)
    delete refraction_texture_;
}

Texture* Material::GetBaseTexture()
{
  return base_texture_;
}

void Material::SetBaseTexture(Texture* texture)
{
  base_texture_ = texture;
}
Texture* Material::GetNormalTexture()
{
  return normal_texture_;
}
void Material::SetNormalTexture(Texture* texture)
{
  normal_texture_ = texture;
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

bool Material::GetReflection()
{
  return reflection_enabled_;
}

void Material::SetReflection(bool value)
{
  reflection_enabled_ = value;
}

bool Material::GetRefraction()
{
  return refraction_enabled_;
}

void Material::SetRefraction(bool value)
{
  refraction_enabled_ = value;
}

float Material::GetRefractionCoefficient()
{
  return refraction_coefficient_;
}

void Material::SetRefractionCoefficient(float value)
{
  refraction_coefficient_ = value;
}

Texture* Material::GetReflectionTexture()
{
  return reflection_texture_;
}
void Material::SetReflectionTexture(Texture* texture)
{
  reflection_texture_ = texture;
}
Texture* Material::GetRefractionTexture()
{
  return refraction_texture_;
}
void Material::SetRefractionTexture(Texture* texture)
{
  refraction_texture_ = texture;
}