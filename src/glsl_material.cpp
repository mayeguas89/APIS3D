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
  if (texture_)
    texture_->Bind();
  render_program_->UseProgram();
  // De momento sabemos las variables harcodeadas
  // Luego podemos obtener de alguna forma el nombre de las variables

  // Guardamos o no los datos de profundidad en el buffer de profundidad que esta activo siempre
  if (IsDepthTestEnabled())
  {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  if (IsCullingEnabled())
  {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  }
  else
  {
    glDisable(GL_CULL_FACE);
  }

  switch (GetBlendMode())
  {
    case BlendMode::Solid:
    {
      glBlendFunc(GL_ONE, GL_ZERO);
      break;
    }
    case BlendMode::Add:
    {
      glBlendFunc(GL_ONE, GL_ONE);
      break;
    }
    case BlendMode::Mul:
    {
      glBlendFunc(GL_DST_COLOR, GL_ZERO);
      break;
    }
    case BlendMode::Alpha:
    {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    }
    default:
      break;
  }

  render_program_->SetInt("shininess", shininess_);
  render_program_->SetFloat("alpha", alpha_);
  render_program_->SetInt("computeLight", (int)IsLightEnabled());
  render_program_->SetInt("hasTexture", 0);
  if (texture_)
    render_program_->SetInt("hasTexture", 1);
  render_program_->SetVariables();
}
