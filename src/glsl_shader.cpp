#include "glsl_shader.h"

#include "common.h"
#include "system.h"
#include "vertex.h"

GLSLShader::GLSLShader(): program_id_{glCreateProgram()} {}

bool GLSLShader::SetProgram(const std::string& filename, RenderType render_type)
{
  auto shader = std::make_unique<Shader>(filename, render_type);
  if (shader->HasErrors(error_message_))
    return false;

  shaders_.push_back(std::move(shader));
  return true;
}

bool GLSLShader::LinkPrograms()
{
  UseProgram();
  for (auto& shader: shaders_)
  {
    glAttachShader(program_id_, shader->shader_id_);
  }
  glLinkProgram(program_id_);
  if (HasErrors(error_message_))
    return false;
  SetupVariableList();
  return true;
}

std::string GLSLShader::GetErrorMsg()
{
  return error_message_;
}

void GLSLShader::UseProgram()
{
  glUseProgram(program_id_);
}

void GLSLShader::SetInt(const std::string& name, int value)
{
  if (variable_list_.find(name) != variable_list_.end())
  {
    glUniform1i(variable_list_[name], value);
  }
}

void GLSLShader::SetFloat(const std::string& name, float value)
{
  if (variable_list_.find(name) != variable_list_.end())
  {
    glUniform1f(variable_list_[name], value);
  }
}

void GLSLShader::SetVec3(const std::string& name, const glm::vec3& value)
{
  if (variable_list_.find(name) != variable_list_.end())
  {
    glUniform3fv(variable_list_[name], 1, glm::value_ptr(value));
  }
}

void GLSLShader::SetVec4(const std::string& name, const glm::vec4& value)
{
  if (variable_list_.find(name) != variable_list_.end())
  {
    glUniform4fv(variable_list_[name], 1, glm::value_ptr(value));
  }
}

void GLSLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
  if (variable_list_.find(name) != variable_list_.end())
  {
    glUniformMatrix4fv(variable_list_[name], 1, GL_FALSE, &(value)[0][0]);
  }
}

void GLSLShader::SetVariables()
{
  // ---------------------Vertex----------------------
  if (variable_list_.find("vPos") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vPos"]);
    glVertexAttribPointer(variable_list_["vPos"], 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0x00);
  }

  if (variable_list_.find("vColor") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vColor"]);
    glVertexAttribPointer(variable_list_["vColor"],
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(sizeof(glm::vec4)));
  }

  if (variable_list_.find("vNormal") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vNormal"]);
    glVertexAttribPointer(variable_list_["vNormal"],
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(2 * sizeof(glm::vec4)));
  }

  if (variable_list_.find("vTangent") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vTangent"]);
    glVertexAttribPointer(variable_list_["vTangent"],
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(3 * sizeof(glm::vec4)));
  }

  if (variable_list_.find("vTexture") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vTexture"]);
    glVertexAttribPointer(variable_list_["vTexture"],
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(4 * sizeof(glm::vec4)));
  }

  if (variable_list_.find("vBoneIndexes") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vBoneIndexes"]);
    glVertexAttribPointer(variable_list_["vBoneIndexes"],
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(4 * sizeof(glm::vec4) + sizeof(glm::vec2)));
  }

  if (variable_list_.find("vBoneWeights") != variable_list_.end())
  {
    glEnableVertexAttribArray(variable_list_["vBoneWeights"]);
    glVertexAttribPointer(variable_list_["vBoneWeights"],
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)(5 * sizeof(glm::vec4) + sizeof(glm::vec2)));
  }

  if (variable_list_.find("mModelMatrix") != variable_list_.end())
  {
    for (int i = 0; i < 4; i++)
    {
      // Enable it
      glEnableVertexAttribArray(variable_list_["mModelMatrix"] + i);
      // Set up the vertex attribute
      glVertexAttribPointer(variable_list_["mModelMatrix"] + i, // Location
                            4,
                            GL_FLOAT,
                            GL_FALSE,                        // vec4
                            sizeof(glm::mat4),               // Stride
                            (void*)(sizeof(glm::vec4) * i)); // Start offset
      // Make it instanced
      glVertexAttribDivisor(variable_list_["mModelMatrix"] + i, 1);
    }
  }

  // ---------------------Ambient Lights----------------------
  if (variable_list_.find("ambient") != variable_list_.end())
  {
    glUniform3fv(variable_list_["ambient"], 1, glm::value_ptr(System::GetAmbient()));
  }

  if (variable_list_.find("ambientIntensity") != variable_list_.end())
  {
    glUniform1f(variable_list_["ambientIntensity"], System::GetAmbientIntensity());
  }

  // ---------------------Camera----------------------
  Camera* camera = System::GetCamera();

  if (variable_list_.find("cameraPosition") != variable_list_.end())
  {
    glUniform3fv(variable_list_["cameraPosition"], 1, glm::value_ptr(glm::vec3(camera->GetPosition())));
  }

  if (variable_list_.find("P") != variable_list_.end())
  {
    glUniformMatrix4fv(variable_list_["P"], 1, GL_FALSE, &camera->GetProjectionMatrix()[0][0]);
  }

  if (variable_list_.find("V") != variable_list_.end())
  {
    glUniformMatrix4fv(variable_list_["V"], 1, GL_FALSE, &camera->GetViewMatrix()[0][0]);
  }

  // ---------------------Lights----------------------

  auto lights = System::GetLights();
  for (auto i = 0; i < lights.size(); i++)
  {
    auto light = lights.at(i);
    std::string index = std::to_string(i);
    if (!light->IsEnabled())
    {
      if (auto key = "Lights[" + index + "].Color"; variable_list_.find(key) != variable_list_.end())
      {
        glUniform3fv(variable_list_[key], 1, &glm::vec3(0.f, 0.f, 0.f)[0]);
      }
      if (auto key = "Lights[" + index + "].IsActive"; variable_list_.find(key) != variable_list_.end())
      {
        SetInt(key, 0);
      }
      continue;
    }

    if (auto key = "Lights[" + index + "].IsActive"; variable_list_.find(key) != variable_list_.end())
    {
      SetInt(key, 1);
    }

    if (auto key = "Lights[" + index + "].Type"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1i(variable_list_[key], light->GetType());
    }

    if (auto key = "Lights[" + index + "].ConstantAttenuation"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetConstantAttenuation());
    }

    if (auto key = "Lights[" + index + "].LinearAttenuation"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetLinearAttenuation());
    }

    if (auto key = "Lights[" + index + "].QuadraticAttenuation"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetQuadraticAttenuation());
    }

    if (auto key = "Lights[" + index + "].AmbientContribution"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetAmbientContribution());
    }

    if (auto key = "Lights[" + index + "].SpecularContribution"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetSpecularContribution());
    }

    if (auto key = "Lights[" + index + "].DifuseContribution"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], light->GetDifuseContribution());
    }

    if (auto key = "Lights[" + index + "].Cutoff"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], glm::cos(glm::radians(light->GetCutOff())));
    }

    if (auto key = "Lights[" + index + "].OuterCutoff"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform1f(variable_list_[key], glm::cos(glm::radians(light->GetOuterCutOff())));
    }

    if (auto key = "Lights[" + index + "].Direction"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform4fv(variable_list_[key], 1, glm::value_ptr(glm::vec4{light->GetDirection(), 0.f}));
    }

    if (auto key = "Lights[" + index + "].Color"; variable_list_.find(key) != variable_list_.end())
    {
      glUniform3fv(variable_list_[key], 1, glm::value_ptr(light->GetColor()));
    }

    if (auto key = "Lights[" + index + "].Position"; variable_list_.find(key) != variable_list_.end())
    {
      SetVec3(key, glm::vec3{light->GetPosition()});
    }
  }

  // ---------------------Object----------------------
  if (variable_list_.find("M") != variable_list_.end())
  {
    glUniformMatrix4fv(variable_list_["M"], 1, GL_FALSE, &(*System::GetModelMatrix())[0][0]);
  }

  // ---------------------Animation----------------------
  SetInt("has_animation", 0);
  if (System::AnimationEnabled())
  {
    SetInt("hasAnimation", 1);
    auto anim_matrices = System::GetAnimationMatrices();
    for (int i = 0; i < anim_matrices.size(); i++)
    {
      auto variable_name = "animationMatrices[" + std::to_string(i) + "]";
      int idarray = glGetUniformLocation(program_id_, variable_name.c_str());
      glUniformMatrix4fv(idarray, 1, GL_FALSE, &anim_matrices[i][0][0]);
    }
  }
}

bool GLSLShader::HasErrors(std::string& error_message)
{
  char error_log[1024];
  GLint link_success;
  glGetProgramiv(program_id_, GL_LINK_STATUS, &link_success);
  if (link_success)
    return false;
  GLsizei log_length = 0;
  glGetProgramInfoLog(program_id_, 1024, &log_length, error_log);
  error_message = std::string("ERROR linking program\n" + std::string(error_log) + "\n\n");
  return true;
}

void GLSLShader::SetupVariableList()
{
  UseProgram();

  int count = 0;
  GLsizei buffSize = 100;
  char* name = new char[buffSize];
  GLsizei length;
  GLint size;
  GLenum type;

  glGetProgramiv(program_id_, GL_ACTIVE_ATTRIBUTES, &count);
  for (int i = 0; i < count; i++)
  {
    glGetActiveAttrib(program_id_, (GLuint)i, 100, &length, &size, &type, name);
    variable_list_[std::string(name)] = glGetAttribLocation(program_id_, name);
  }

  glGetProgramiv(program_id_, GL_ACTIVE_UNIFORMS, &count);
  for (int i = 0; i < count; i++)
  {
    glGetActiveUniform(program_id_, (GLuint)i, 100, &length, &size, &type, name);
    variable_list_[std::string(name)] = glGetUniformLocation(program_id_, name);
  }
}

/**
 * @brief Construct a new GLSLShader::Shader::Shader object
 * 
 * Read the file content
 * Creates the shader_id
 * Compile the program
 * 
 * @param filename 
 * @param render_type 
 */
GLSLShader::Shader::Shader(const std::string& filename, RenderType render_type): filename_{filename}
{
  switch (render_type)
  {
    case RenderType::Vertex:
    {
      type_ = GL_VERTEX_SHADER;
      break;
    }
    case RenderType::Fragment:
    {
      type_ = GL_FRAGMENT_SHADER;
      break;
    }
    default:
    {
      type_ = -1;
      break;
    }
  }
  ReadSource();
  CompileShader();
}

void GLSLShader::Shader::ReadSource()
{
  std::streampos size;
  std::ifstream file(filename_, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open())
  {
    std::cout << "Unable to open file " << filename_;
    return;
  }
  size = file.tellg();
  source_ = new char[size + std::streampos(1)];
  file.seekg(0, std::ios::beg);
  file.read(source_, size);
  source_[size] = '\0';
  file.close();
}

void GLSLShader::Shader::CompileShader()
{
  shader_id_ = glCreateShader(type_);
  glShaderSource(shader_id_, 1, &source_, nullptr);
  glCompileShader(shader_id_);
}

bool GLSLShader::Shader::HasErrors(std::string& error_message)
{
  char error_log[1024];
  GLint compilation_succes;
  glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &compilation_succes);
  if (compilation_succes)
    return false;
  GLsizei log_length = 0;
  glGetShaderInfoLog(shader_id_, 1024, &log_length, error_log);
  error_message = std::string("ERROR compiling shader " + filename_ + "\n" + std::string(error_log) + "\n\n");
  return true;
}

void GLSLShader::Shader::Clean() {}
