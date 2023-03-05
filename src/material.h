#pragma once

#include "render_program.h"
#include "texture.h"

class Material
{
public:
  enum class BlendMode
  {
    Solid = 0,
    Add,
    Mul,
    Alpha,
    None,
  };

  Material();
  ~Material();
  Texture* GetBaseTexture();
  void SetBaseTexture(Texture* texture);
  Texture* GetNormalTexture();
  void SetNormalTexture(Texture* texture);
  Texture* GetReflectionTexture();
  void SetReflectionTexture(Texture* texture);
  Texture* GetRefractionTexture();
  void SetRefractionTexture(Texture* texture);

  /**
   * @brief Metodo que recibe una lista de programas, los pasa al render program para que los compile y los linke
   * 
   */
  virtual void LoadPrograms(std::unordered_map<std::string, RenderType>& program_map) = 0;

  /**
   * @brief Metodo que inicializa las variables del RenderProgram
   * 
   */
  virtual void Prepare() = 0;

  void SetColor(const glm::vec3& color);
  void SetShininess(uint8_t shininess);

  const glm::vec3& GetColor() const;
  uint8_t GetShininess() const;

  bool IsLightEnabled();
  bool IsCullingEnabled();
  bool IsDepthTestEnabled();

  void SetLightEnabled(bool value);
  void SetCullingEnabled(bool value);
  void SetDepthTestEnabled(bool value);

  BlendMode GetBlendMode();
  void SetBlendMode(BlendMode value);
  void SetAlpha(float alpha);
  float GetAlpha();

  bool GetReflection();
  void SetReflection(bool value);
  bool GetRefraction();
  void SetRefraction(bool value);
  float GetRefractionCoefficient();
  void SetRefractionCoefficient(float value);

protected:
  RenderProgram* render_program_ = nullptr;
  Texture* base_texture_ = nullptr;
  Texture* normal_texture_ = nullptr;
  Texture* reflection_texture_ = nullptr;
  Texture* refraction_texture_ = nullptr;
  glm::vec3 color_;
  uint8_t shininess_;
  bool light_enabled_ = true;
  bool culling_enabled_ = false;
  bool depth_test_enabled_ = true;
  bool reflection_enabled_ = false;
  bool refraction_enabled_ = false;
  float refraction_coefficient_ = 1.0;

  float alpha_ = 1.0f;
  BlendMode blend_mode_ = BlendMode::None;
};
