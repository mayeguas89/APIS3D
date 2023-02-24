#pragma once

#include "render_program.h"
#include "texture.h"

class Material
{
public:
  Material();
  ~Material();
  Texture* GetTexture();
  void SetTexture(Texture* texture);

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

protected:
  RenderProgram* render_program_;
  Texture* texture_;
  glm::vec3 color_;
  uint8_t shininess_;
};
