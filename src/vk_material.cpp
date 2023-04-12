#include "vk_material.h"

#include "vulkan_shader.h"

VkMaterial::VkMaterial()
{
  render_program_ = new VulkanShader();
  shadow_program_ = nullptr;
}

VkMaterial::~VkMaterial() {}

void VkMaterial::LoadPrograms(std::unordered_map<std::string, RenderType>& program_map)
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
}

void VkMaterial::Prepare() {}
