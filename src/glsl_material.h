#pragma once

#include "material.h"

class GLSLMaterial: public Material
{
public:
  GLSLMaterial();
  ~GLSLMaterial();

  void LoadPrograms(std::unordered_map<std::string, RenderType>& program_map) override;
  void Prepare() override;
};
