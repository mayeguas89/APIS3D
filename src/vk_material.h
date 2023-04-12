#pragma once

#include "material.h"

class VkMaterial: public Material
{
public:
  VkMaterial();
  ~VkMaterial();

  void LoadPrograms(std::unordered_map<std::string, RenderType>& program_map) override;
  void Prepare() override;
};