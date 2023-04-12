#pragma once

#ifdef __GNUC__
#define LINUX
#else
#define WINDOWS
#endif

#ifdef WINDOWS
#define _fopen(f, fn, m) fopen_s(&f, fn, m)
#else
#define _fopen(f, fn, m) f = fopen(fn, m)
#endif

#ifndef __DEBUG

#include "render_program.h"
#include "shaderc/shaderc.hpp"

class VulkanShader: public RenderProgram
{
public:
  VulkanShader();
  bool SetProgram(const std::string& filename, RenderType render_type) override;
  bool LinkPrograms() override;
  std::string GetErrorMsg() override;
  void UseProgram() override;

  void SetInt(const std::string& name, int value) override;
  void SetFloat(const std::string& name, float value) override;
  void SetVec3(const std::string& name, const glm::vec3& value) override;
  void SetVec4(const std::string& name, const glm::vec4& value) override;
  void SetMat4(const std::string& name, const glm::mat4& value) override;

  void SetVariables() override;

private:
  class Shader
  {
  public:
    Shader(const std::string& filename, RenderType render_type);
    void ReadSource();
    void CompileShader();
    bool HasErrors(std::string& error_message);
    void Clean();

    // Identificador del shader
    unsigned int shader_id_;
    // Tipo de shader
    shaderc_shader_kind type_;
    // Fichero del shader
    std::string filename_;
    // codigo fuente del fichero
    char* source_;

    uint32_t length_;
    uint32_t* data_;
  };

  // Error message
  std::string error_message_;

  // Contenedor de shaders
  std::vector<std::unique_ptr<Shader>> shaders_;
};

#endif