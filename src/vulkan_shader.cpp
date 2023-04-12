#include "vulkan_shader.h"

#include <fstream>
#include <iostream>

VulkanShader::Shader::Shader(const std::string& filename, RenderType render_type): filename_{filename}
{
  switch (render_type)
  {
    case RenderType::Vertex:
    {
      type_ = shaderc_glsl_vertex_shader;
      break;
    }
    case RenderType::Fragment:
    {
      type_ = shaderc_glsl_fragment_shader;
      break;
    }
    default:
    {
      type_ = shaderc_glsl_default_vertex_shader;
      break;
    }
  }
  ReadSource();
  CompileShader();
}

void VulkanShader::Shader::ReadSource()
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

void VulkanShader::Shader::CompileShader()
{
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;

  shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source_, type_, filename_.c_str(), options);

  if (module.GetCompilationStatus() != shaderc_compilation_status_success)
  {
    std::cerr << "ERROR: " << filename_ << "\n";
    std::cerr << module.GetErrorMessage();
    data_ = NULL;
    length_ = 0;
  }

  std::vector<uint32_t> v = {module.cbegin(), module.cend()};
  length_ = v.size();
  data_ = new uint32_t[length_];

  memcpy(data_, v.data(), sizeof(uint32_t) * length_);
}

void VulkanShader::Shader::Clean() {}

bool VulkanShader::Shader::HasErrors(std::string& error_message)
{
  return false;
}

VulkanShader::VulkanShader() {}

bool VulkanShader::SetProgram(const std::string& filename, RenderType render_type)
{
  auto shader = std::make_unique<Shader>(filename, render_type);
  if (shader->HasErrors(error_message_))
    return false;

  shaders_.push_back(std::move(shader));
  return true;
}

bool VulkanShader::LinkPrograms()
{
  return false;
}

std::string VulkanShader::GetErrorMsg()
{
  return error_message_;
}

void VulkanShader::UseProgram() {}

void VulkanShader::SetInt(const std::string& name, int value) {}

void VulkanShader::SetFloat(const std::string& name, float value) {}

void VulkanShader::SetVec3(const std::string& name, const glm::vec3& value) {}

void VulkanShader::SetVec4(const std::string& name, const glm::vec4& value) {}

void VulkanShader::SetMat4(const std::string& name, const glm::mat4& value) {}

void VulkanShader::SetVariables() {}
