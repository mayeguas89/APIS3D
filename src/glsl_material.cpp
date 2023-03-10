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

  // if (IsCullingEnabled())
  // {
  //   glEnable(GL_CULL_FACE);
  //   glCullFace(GL_BACK);
  // }
  // else
  // {
  //   glDisable(GL_CULL_FACE);
  // }

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
  render_program_->SetInt("computeLight", light_enabled_);
  render_program_->SetFloat("alpha", alpha_);

  render_program_->SetInt("computeReflection", reflection_enabled_);
  render_program_->SetInt("computeRefraction", refraction_enabled_);
  render_program_->SetFloat("refractCoef", refraction_coefficient_);

  if (base_texture_ == nullptr)
  {
    render_program_->SetInt("colorTexture", 0);

    if (reflection_texture_ || refraction_texture_)
    {
      auto texture = (reflection_texture_) ? reflection_texture_ : refraction_texture_;
      render_program_->SetInt("hasTexture", 1);
      render_program_->SetInt("cubeTexture", 1);
      texture->Bind(1);
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
    else
    {
      render_program_->SetInt("hasTexture", 0);
      render_program_->SetInt("cubeTexture", 1);
    }
  }
  else
  {
    render_program_->SetInt("hasTexture", 1);

    switch (base_texture_->GetType())
    {
      case Texture::Type::kCubeMap:
        base_texture_->Bind(1);
        render_program_->SetInt("isCubeMap", 1);
        // Samplers
        render_program_->SetInt("colorTexture", 0);
        render_program_->SetInt("cubeTexture", 1);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        break;
      case Texture::Type::kColor2D:
        base_texture_->Bind(0);
        render_program_->SetInt("isCubeMap", 0);
        // Samplers
        render_program_->SetInt("colorTexture", 0);
        render_program_->SetInt("cubeTexture", 1);
        glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        break;
      case Texture::Type::kNone:
      case Texture::Type::kNormal:
        break;
    }
  }

  render_program_->SetInt("computeNormalTexture", 0);
  render_program_->SetInt("normalTexture", 2);
  if (normal_texture_)
  {
    normal_texture_->Bind(2);
    render_program_->SetInt("computeNormalTexture", 1);
    render_program_->SetInt("normalTexture", 2);
  }

  render_program_->SetVariables();
}
