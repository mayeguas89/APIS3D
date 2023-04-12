#include "vk_texture.h"

#include "stb_image.h"
#include "vulkan_helpers.h"

VkTexture::VkTexture() {}

VkTexture::~VkTexture()
{
  vkDestroySampler(VulkanContext::device, texture_sampler, nullptr);
  vkDestroyImageView(VulkanContext::device, texture_image_view, nullptr);
  vkDestroyImage(VulkanContext::device, texture_image, nullptr);
  vkFreeMemory(VulkanContext::device, texture_image_memory, nullptr);
}

bool VkTexture::Load(const std::vector<std::string>& filenames, Type type)
{
  int width, height, texture_channels;
  stbi_uc* pixels = stbi_load(filenames[0].c_str(), &width, &height, &texture_channels, STBI_rgb_alpha);
  VkDeviceSize image_size = width * height * 4;
  if (pixels == nullptr)
    throw std::runtime_error("failed to load texture image!");

  vulkan_helpers::CreateBuffer(VulkanContext::device,
                               VulkanContext::physical_device,
                               image_size,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               staging_buffer,
                               staging_buffer_memory);
  void* data;
  vkMapMemory(VulkanContext::device, staging_buffer_memory, 0, image_size, 0, &data);
  memcpy(data, pixels, static_cast<size_t>(image_size));
  vkUnmapMemory(VulkanContext::device, staging_buffer_memory);

  vulkan_helpers::CreateImage(width,
                              height,
                              VK_FORMAT_R8G8B8A8_SRGB,
                              VK_IMAGE_TILING_OPTIMAL,
                              VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                              texture_image,
                              texture_image_memory);
  VulkanContext::TransitionImageLayout(texture_image,
                                       VK_FORMAT_R8G8B8A8_SRGB,
                                       VK_IMAGE_LAYOUT_UNDEFINED,
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  VulkanContext::CopyBufferToImage(staging_buffer,
                                   texture_image,
                                   static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height));
  VulkanContext::TransitionImageLayout(texture_image,
                                       VK_FORMAT_R8G8B8A8_SRGB,
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                       VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  vkDestroyBuffer(VulkanContext::device, staging_buffer, nullptr);
  vkFreeMemory(VulkanContext::device, staging_buffer_memory, nullptr);

  stbi_image_free(pixels);

  CreateTextureImageView();
  CreateTextureSampler();
  return true;
}

void VkTexture::Bind(unsigned int index) {}

void VkTexture::CreateTextureImageView()
{
  VkImageViewCreateInfo view_info{};
  view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_info.image = texture_image;
  view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
  view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
  view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  view_info.subresourceRange.baseMipLevel = 0;
  view_info.subresourceRange.levelCount = 1;
  view_info.subresourceRange.baseArrayLayer = 0;
  view_info.subresourceRange.layerCount = 1;
  if (vkCreateImageView(VulkanContext::device, &view_info, nullptr, &texture_image_view) != VK_SUCCESS)
    throw std::runtime_error("failed to create texture image view!");
}

void VkTexture::CreateTextureSampler()
{
  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(VulkanContext::physical_device, &properties);
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.minLod = 0.0f;
  samplerInfo.maxLod = 0.0f;
  if (vkCreateSampler(VulkanContext::device, &samplerInfo, nullptr, &texture_sampler) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create texture sampler!");
  }
}
