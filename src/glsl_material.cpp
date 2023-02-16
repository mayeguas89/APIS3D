#include "glsl_material.h"

#include "glsl_shader.h"

GLSLMaterial::GLSLMaterial(): Material()
{
  render_program_ = new GLSLShader();
}

GLSLMaterial::~GLSLMaterial()
{
  // delete render_program_;
}

/**
 * @brief Metodo que recibe una lista de programas, los pasa al render program para que los compile y los linke
 * 
 */
void GLSLMaterial::LoadPrograms(std::unordered_map<std::string, RenderType>& program_map)
{
  for (auto it = program_map.begin(); it != program_map.end(); it++)
  {
    // Comprobamos si el fichero existe
    if (!std::filesystem::exists(it->first))
      continue;

    // Detectar errores en compilacion del shader
    if (auto success = render_program_->SetProgram(it->first, it->second); !success)
      std::cout << render_program_->GetErrorMsg();
  }

  // Si hay algun error en los shader no continuamos
  if (!render_program_->GetErrorMsg().empty())
    return;

  // Si hay algun error en el linkado no continuamos
  if (!render_program_->LinkPrograms())
  {
    std::cout << render_program_->GetErrorMsg();
    return;
  }
}

/**
 * @brief Metodo que inicializa las variables del RenderProgram
 * 
 */
void GLSLMaterial::Prepare()
{
  texture_->Bind();
  render_program_->UseProgram();
  // De momento sabemos las variables harcodeadas
  // Luego podemos obtener de alguna forma el nombre de las variables
  render_program_->SetVariables();
}
