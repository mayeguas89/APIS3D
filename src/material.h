#pragma once

#include "render_program.h"

class Material
{
public:
  Material();
  virtual ~Material() = default;

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

protected:
  RenderProgram* render_program_;
};
