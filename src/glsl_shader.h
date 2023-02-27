#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "render_program.h"

class GLSLShader: public RenderProgram
{
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
    GLenum type_;
    // Fichero del shader
    std::string filename_;
    // codigo fuente del fichero
    char* source_;
  };

public:
  GLSLShader();

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
  bool HasErrors(std::string& error_message);
  void SetupVariableList();

  // Error message
  std::string error_message_;

  // Identificador del programa
  unsigned int program_id_;

  // Contenedor de shaders
  std::vector<std::unique_ptr<Shader>> shaders_;

  // Variables que el programa almacena de los shaders
  std::unordered_map<std::string, int> variable_list_;
};