#include "vulkan_helpers.h"

namespace vulkan_helpers
{
uint32_t FindMemoryType(const VkPhysicalDevice& device, uint32_t type_filter, VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(device, &mem_properties);

  for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
  {
    if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
    {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

void CreateBuffer(VkDevice device,
                  VkPhysicalDevice physical_device,
                  VkDeviceSize size,
                  VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties,
                  VkBuffer& buffer,
                  VkDeviceMemory& buffer_memory)
{
  VkBufferCreateInfo buffer_info{};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = size;
  buffer_info.usage = usage;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &buffer_info, nullptr, &buffer) != VK_SUCCESS)
    throw std::runtime_error("failed to create vertex buffer!");

  VkMemoryRequirements memory_requirements;
  vkGetBufferMemoryRequirements(device, buffer, &memory_requirements);

  VkMemoryAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = memory_requirements.size;
  alloc_info.memoryTypeIndex =
    FindMemoryType(physical_device,
                   memory_requirements.memoryTypeBits,
                   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS)
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  vkBindBufferMemory(device, buffer, buffer_memory, 0);
}

std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescription()
{
  // An attribute description struct describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
  std::array<VkVertexInputAttributeDescription, 2> attribute_description;
  // Position
  attribute_description[0].binding = 0;
  attribute_description[0].location = 0;
  attribute_description[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attribute_description[0].offset = offsetof(Vertex, position);
  // Color
  attribute_description[1].binding = 0;
  attribute_description[1].location = 1;
  attribute_description[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attribute_description[1].offset = offsetof(Vertex, color);
  return attribute_description;
}

VkVertexInputBindingDescription GetBindingDescription()
{
  VkVertexInputBindingDescription vertex_input_description;
  vertex_input_description.binding = 0;
  // number of bytes from one entry to the next
  vertex_input_description.stride = sizeof(Vertex);
  vertex_input_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return vertex_input_description;
}

VkShaderModule CreateShaderModule(const std::vector<char>& code, const VkDevice& device)
{
  VkShaderModuleCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
  VkShaderModule shader_module;
  if (vkCreateShaderModule(device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
    throw std::runtime_error("failed to create shader module!");

  return shader_module;
}

std::vector<char> ReadSource(const std::string& filename)
{
  std::streampos size;
  std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open())
  {
    std::cout << "Unable to open file " << filename << std::endl;
    return {};
  }
  size = file.tellg();
  std::vector<char> buffer(size);
  file.seekg(0, std::ios::beg);
  file.read(buffer.data(), size);
  file.close();
  return buffer;
}

VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
  {
    return capabilities.currentExtent;
  }
  else
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width =
      std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height =
      std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
{
  for (const auto& available_present_mode: available_present_modes)
  {
    if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
      return available_present_mode;
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
  for (const auto& available_format: available_formats)
  {
    if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB
        && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return available_format;
  }

  return available_formats[0];
}

VulkanContext::SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device,
                                                             const VkSurfaceKHR& surface)
{
  // Let's start with the basic surface capabilities
  VulkanContext::SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  // Querying the supported surface formats
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

  if (format_count != 0)
  {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
  }

  // querying the supported presentation modes
  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

  if (present_mode_count != 0)
  {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
  }
  return details;
}

std::vector<const char*> GetRequiredExtensions()
{
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  if (VulkanContext::kEnableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return extensions;
}

bool CheckDeviceExtensionSupported(const VkPhysicalDevice& device)
{
  uint32_t extension_count;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

  std::set<std::string> required_extensions(VulkanContext::kDeviceExtensions.begin(),
                                            VulkanContext::kDeviceExtensions.end());

  for (const auto& extension: available_extensions)
  {
    required_extensions.erase(extension.extensionName);
  }

  return required_extensions.empty();
}

bool CheckValidationLayerSupport()
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const char* layerName: VulkanContext::kValidationLayers)
  {
    bool layer_found = false;

    for (const auto& layer_properties: available_layers)
    {
      if (strcmp(layerName, layer_properties.layerName) == 0)
      {
        layer_found = true;
        break;
      }
    }

    if (!layer_found)
    {
      return false;
    }
  }

  return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void* pUserData)
{
  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                               | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                           | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = DebugCallback;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger)
{
  auto func =
    (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator)
{
  auto func =
    (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr)
  {
    func(instance, debugMessenger, pAllocator);
  }
}

VulkanContext::QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
{
  VulkanContext::QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

  int i = 0;
  for (const auto& queue_family: queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      indices.graphics_family = i;
    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
    if (present_support)
      indices.present_family = i;
    if (indices.IsComplete())
      break;
    i++;
  }
  return indices;
}

bool IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface)
{
  VulkanContext::QueueFamilyIndices indices = FindQueueFamilies(device, surface);

  bool extension_supported = CheckDeviceExtensionSupported(device);

  bool swap_chain_adequate = false;
  if (extension_supported)
  {
    VulkanContext::SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
    swap_chain_adequate = !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
  }

  return indices.IsComplete() && extension_supported && swap_chain_adequate;
}

}