#pragma once

#define GLAD_ONLY_HEADER
#define GLFW_INCLUDE_VULKAN
#include "common.h"
#include "vulkan/vulkan.h"

#include <optional>
#include <vector>

struct VulkanContext
{
#ifdef NDEBUG
  inline static const bool kEnableValidationLayers = false;
#else
  inline static const bool kEnableValidationLayers = true;
#endif
  inline static const std::vector<const char*> kValidationLayers = {"VK_LAYER_KHRONOS_validation"};
  inline static const std::vector<const char*> kDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  inline static int kMaxBufferInFlight = 2;

  struct QueueFamilyIndices
  {
    // Family queue able to draw
    std::optional<uint32_t> graphics_family;
    // Family queue able to present
    std::optional<uint32_t> present_family;

    bool IsComplete()
    {
      return graphics_family.has_value() && present_family.has_value();
    }
  };

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  void Init(GLFWwindow* window);

  // -------- Functions -----------
  void ClearInstance();
  void CleanupSwapChain();

  void CreateInstance();
  void SetupDebugMessenger();
  void CreateSurface(GLFWwindow* window);
  void PickPhysicalDevice();
  void CreateLogicalDevice();
  void CreateSwapChain(GLFWwindow* window);
  void CreateImageViews();
  void CreateRenderPass();
  void CreateGraphicsPipeline();
  void CreateFramebuffers();
  void CreateCommandPool();
  void CreateCommandBuffers();
  void CreateSyncObjects();

  void RecordCommandBuffer(const VkCommandBuffer& command_buffer, uint32_t imageIndex);
  void RecreateSwapChain(GLFWwindow* window);

  VkInstance instance;
  VkDebugUtilsMessengerEXT debug_messenger;
  VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  VkSurfaceKHR surface;
  VkDevice device;
  VkQueue graphics_queue;
  VkQueue present_queue;
  VkSwapchainKHR swap_chain;
  std::vector<VkImage> swap_chain_images;
  VkFormat swap_chain_image_format;
  VkExtent2D swap_chain_extent;
  std::vector<VkImageView> swap_chain_image_views;
  VkRenderPass render_pass;
  VkPipelineLayout pipeline_layout;
  VkPipeline graphics_pipeline;
  std::vector<VkFramebuffer> swap_chain_framebuffers;
  VkCommandPool command_pool;
  std::vector<VkCommandBuffer> command_buffers;
  std::vector<VkSemaphore> image_available_semaphores;
  std::vector<VkSemaphore> render_finished_semaphores;
  std::vector<VkFence> in_flight_fences;
  // Handle window resize
  bool frame_buffer_resized = false;
};