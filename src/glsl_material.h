#pragma once

#include "material.h"

class GLSLMaterial: public Material
{
public:
  GLSLMaterial();
  ~GLSLMaterial();

  void LoadPrograms(std::unordered_map<std::string, RenderType>& program_map) override;
  void Prepare() override;

private:
  inline static const std::string kShadowVertexProgram{"data/shadow.vertex"};
  inline static const std::string kShadowFragmentProgram{"data/shadow.fragment"};
  void PrepareShadowVariables();
  void PrepareColorVariables();
  void PrepareCommonVariables();
  void LoadShadowPrograms();
};
