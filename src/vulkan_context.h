#pragma once

#define GLAD_ONLY_HEADER
#define GLFW_INCLUDE_VULKAN
#include "common.h"
#include "vertex.h"
#include "vulkan/vulkan.h"

#include <map>
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

  const std::vector<Vertex> kVertices = {{{0.0f, -0.5f, 0.f, 1.f}, {1.0f, 0.0f, 0.0f, 1.f}, {}, {}, {}, {}, {}},
                                         {{0.5f, 0.5f, 0.f, 1.f}, {0.0f, 1.0f, 0.0f, 1.f}, {}, {}, {}, {}, {}},
                                         {{-0.5f, 0.5f, 0.f, 1.f}, {0.0f, 0.0f, 1.0f, 1.f}, {}, {}, {}, {}, {}}};

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

  struct VertexResources
  {
    VkBuffer buffer;
    VkDeviceMemory memory;
    std::optional<uint32_t> num_elements;
  };

  struct UniformResources
  {
    // We should have multiple buffers,
    // because multiple frames may be in flight at the same time
    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceMemory> buffers_memory;
    std::vector<void*> buffers_mapped;
  };

  struct UniformBufferObject
  {
    glm::mat4 model_matrix;
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
  };

  static void Init(GLFWwindow* window);

  // -------- Functions -----------
  static void ClearInstance();
  static void CleanupSwapChain();

  static void CreateInstance();
  static void SetupDebugMessenger();
  static void CreateSurface(GLFWwindow* window);
  static void PickPhysicalDevice();
  static void CreateLogicalDevice();
  static void CreateSwapChain(GLFWwindow* window);
  static void CreateImageViews();
  static void CreateRenderPass();
  static void CreateDescriptorSetLayout();
  static void CreateGraphicsPipeline();
  static void CreateGraphicsPipeline(const std::vector<VkPipelineShaderStageCreateInfo>& shader_stage_vector);
  static void CreateFramebuffers();
  static void CreateCommandPool();
  static void CreateVertexBuffer(int id, std::vector<Vertex>* vertices);
  static void CreateIndexBuffer(int id, std::vector<unsigned int>* indices);
  static void CreateUniformBuffers();
  static void CreateDescriptorPool();
  static void CreateDescriptorSets(VkImageView image_view, VkSampler sampler);
  static void CreateCommandBuffers();
  static void CreateSyncObjects();

  static void RecordCommandBuffer(uint32_t current_frame, uint32_t image_index);
  static void RecreateSwapChain(GLFWwindow* window);
  static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  static void
    TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout);
  static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

  inline static VkInstance instance;
  inline static VkDebugUtilsMessengerEXT debug_messenger;
  inline static VkPhysicalDevice physical_device = VK_NULL_HANDLE;
  inline static VkSurfaceKHR surface;
  inline static VkDevice device;
  inline static VkQueue graphics_queue;
  inline static VkQueue present_queue;
  inline static VkSwapchainKHR swap_chain;
  inline static std::vector<VkImage> swap_chain_images;
  inline static VkFormat swap_chain_image_format;
  inline static VkExtent2D swap_chain_extent;
  inline static std::vector<VkImageView> swap_chain_image_views;
  inline static VkRenderPass render_pass;
  inline static VkDescriptorSetLayout descriptor_set_layout;
  inline static VkPipelineLayout pipeline_layout;
  inline static VkPipeline graphics_pipeline;
  inline static std::vector<VkFramebuffer> swap_chain_framebuffers;
  inline static VkCommandPool command_pool;
  inline static std::vector<VkCommandBuffer> command_buffers;
  inline static std::vector<VkSemaphore> image_available_semaphores;
  inline static std::vector<VkSemaphore> render_finished_semaphores;
  inline static std::vector<VkFence> in_flight_fences;
  inline static std::map<int, VertexResources> vertex_buffer_map;
  inline static std::map<int, VertexResources> indices_buffer_map;
  inline static UniformResources uniform_resources;
  inline static VkDescriptorPool descriptor_pool;
  inline static std::vector<VkDescriptorSet> descriptor_sets;
  // Handle window resize
  inline static bool frame_buffer_resized = false;
};