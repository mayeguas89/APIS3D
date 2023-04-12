#pragma once
#include "vulkan/vulkan.h"
#include "vulkan_context.h"

#define GLFW_INCLUDE_VULKAN
#include "vertex.h"

#include <GLFW/glfw3.h>

#include <array>
#include <set>
#include <string>

namespace vulkan_helpers
{

void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

VkCommandBuffer BeginSingleTimeCommands();

void CreateImage(uint32_t width,
                 uint32_t height,
                 VkFormat format,
                 VkImageTiling tiling,
                 VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties,
                 VkImage& image,
                 VkDeviceMemory& imageMemory);
uint32_t FindMemoryType(const VkPhysicalDevice& device, uint32_t type_filter, VkMemoryPropertyFlags properties);

void CreateBuffer(const VkDevice& device,
                  const VkPhysicalDevice& physical_device,
                  VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkBuffer& buffer,
                  VkDeviceMemory& buffer_memory);
std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescription();
VkVertexInputBindingDescription GetBindingDescription();

VkShaderModule CreateShaderModule(const std::vector<char>& code, const VkDevice& device);

std::vector<char> ReadSource(const std::string& filename);

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);
VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
VulkanContext::SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device,
                                                             const VkSurfaceKHR& surface);

std::vector<const char*> GetRequiredExtensions();

bool CheckDeviceExtensionSupported(const VkPhysicalDevice& device);

bool CheckValidationLayerSupport();

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void* pUserData);

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);

VulkanContext::QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

bool IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

}