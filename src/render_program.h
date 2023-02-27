#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"

enum class RenderType
{
  None = -1,
  Vertex = 0,
  Fragment = 1,
};

class RenderProgram
{
public:
  virtual ~RenderProgram() = default;
  virtual bool SetProgram(const std::string& filename, RenderType render_type) = 0;
  virtual bool LinkPrograms() = 0;
  virtual std::string GetErrorMsg() = 0;
  virtual void UseProgram() = 0;

  virtual void SetInt(const std::string& name, int value) = 0;
  virtual void SetFloat(const std::string& name, float value) = 0;
  virtual void SetVec3(const std::string& name, const glm::vec3& value) = 0;
  virtual void SetVec4(const std::string& name, const glm::vec4& value) = 0;
  virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

  virtual void SetVariables() = 0;
};