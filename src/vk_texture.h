#pragma once
#include "texture.h"
#include "vulkan/vulkan.h"
class VkTexture: public Texture
{
public:
  VkTexture();
  ~VkTexture();
  bool Load(const std::vector<std::string>& filenames, Type type = Type::kColor2D) override;
  void Bind(unsigned int index = 0) override;
  const VkImageView& GetImageView() const
  {
    return texture_image_view;
  }
  const VkSampler& GetImageSampler() const
  {
    return texture_sampler;
  }

private:
  void CreateTextureImageView();
  void CreateTextureSampler();

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_memory;

  VkImage texture_image;
  VkDeviceMemory texture_image_memory;
  VkImageView texture_image_view;
  VkSampler texture_sampler;
};