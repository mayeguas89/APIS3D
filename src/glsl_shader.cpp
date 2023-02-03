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

void GLSLShader::SetInt(const std::string& name, int value) {}

void GLSLShader::SetFloat(const std::string& name, int value) {}

void GLSLShader::SetVec3(const std::string& name, const glm::vec3& value) {}

void GLSLShader::SetVec4(const std::string& name, const glm::vec4& value) {}

void GLSLShader::SetMat4(const std::string& name, const glm::mat4& value)
{
  glUniformMatrix4fv(variable_list_[name], 1, GL_FALSE, &(value)[0][0]);
}

void GLSLShader::SetVariables()
{
  glEnableVertexAttribArray(variable_list_["vPos"]);
  glVertexAttribPointer(variable_list_["vPos"], 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0x00);

  glEnableVertexAttribArray(variable_list_["vColor"]);
  glVertexAttribPointer(variable_list_["vColor"],
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void*)(sizeof(glm::vec4)));

  glUniformMatrix4fv(variable_list_["MVP"], 1, GL_FALSE, &(*System::GetModelMatrix())[0][0]);
}

bool GLSLShader::HasErrors(std::string& error_message)
{
  std::string error_log;
  GLint link_success;
  glGetProgramiv(program_id_, GL_LINK_STATUS, &link_success);
  if (link_success)
    return false;
  GLsizei log_length = 0;
  glGetProgramInfoLog(program_id_, 1024, &log_length, error_log.data());
  error_message = std::string("ERROR linking program\n" + error_log + "\n\n");
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
  std::string error_log;
  //   char error_log[1024];
  GLint compilation_succes;
  glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &compilation_succes);
  if (compilation_succes)
    return false;
  GLsizei log_length = 0;
  glGetShaderInfoLog(shader_id_, 1024, &log_length, error_log.data());
  error_message = std::string("ERROR compiling shader " + filename_ + "\n" + error_log + "\n\n");
  return true;
}

void GLSLShader::Shader::Clean() {}
