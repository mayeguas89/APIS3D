#include "vulkan_context.h"

#include "vulkan_helpers.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <set>

void VulkanContext::ClearInstance()
{
  CleanupSwapChain();

  vkDestroyPipeline(device, graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(device, pipeline_layout, nullptr);

  vkDestroyRenderPass(device, render_pass, nullptr);

  for (int i = 0; i < kMaxBufferInFlight; i++)
  {
    vkDestroySemaphore(device, image_available_semaphores[i], nullptr);
    vkDestroySemaphore(device, render_finished_semaphores[i], nullptr);
    vkDestroyFence(device, in_flight_fences[i], nullptr);
  }

  vkDestroyCommandPool(device, command_pool, nullptr);
  vkDestroyDevice(device, nullptr);

  if (kEnableValidationLayers)
    vulkan_helpers::DestroyDebugUtilsMessengerEXT(instance, debug_messenger, nullptr);

  vkDestroySurfaceKHR(instance, surface, nullptr);

  vkDestroyInstance(instance, nullptr);
}

void VulkanContext::CleanupSwapChain()
{
  for (auto framebuffer: swap_chain_framebuffers)
    vkDestroyFramebuffer(device, framebuffer, nullptr);
  for (auto image_view: swap_chain_image_views)
    vkDestroyImageView(device, image_view, nullptr);
  vkDestroySwapchainKHR(device, swap_chain, nullptr);
}

void VulkanContext::Init(GLFWwindow* window)
{
  CreateInstance();
  SetupDebugMessenger();
  CreateSurface(window);
  PickPhysicalDevice();
  CreateLogicalDevice();
  CreateSwapChain(window);
  CreateImageViews();
  CreateRenderPass();
  CreateGraphicsPipeline();
  CreateFramebuffers();
  CreateCommandPool();
  CreateCommandBuffers();
  CreateSyncObjects();
}

void VulkanContext::CreateInstance()
{
  if (kEnableValidationLayers && !vulkan_helpers::CheckValidationLayerSupport())
    throw std::runtime_error("validation layers requested, but not available!");

  //  fill in a struct with some information about our application -- optional --
  VkApplicationInfo app_info{};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Graphics Engine";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  // specify the desired global extensions
  // extension to interface with the window system though GLFW
  auto extensions = vulkan_helpers::GetRequiredExtensions();
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();

  // determine the global validation layers to enable

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
  if (kEnableValidationLayers)
  {
    create_info.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
    create_info.ppEnabledLayerNames = kValidationLayers.data();

    vulkan_helpers::PopulateDebugMessengerCreateInfo(debug_create_info);
    create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
  }
  else
  {
    create_info.enabledLayerCount = 0;

    create_info.pNext = nullptr;
  }

  // create an instance
  if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS)
    throw std::runtime_error("Failed to create vulkan instance!");
}

void VulkanContext::SetupDebugMessenger()
{
  if (!kEnableValidationLayers)
    return;

  VkDebugUtilsMessengerCreateInfoEXT create_info;
  vulkan_helpers::PopulateDebugMessengerCreateInfo(create_info);

  if (vulkan_helpers::CreateDebugUtilsMessengerEXT(instance, &create_info, nullptr, &debug_messenger)
      != VK_SUCCESS)
    throw std::runtime_error("failed to set up debug messenger!");
}

void VulkanContext::CreateSurface(GLFWwindow* window)
{
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    throw std::runtime_error("failed to create window surface!");
}

void VulkanContext::PickPhysicalDevice()
{
  // Listing the graphics cards
  uint32_t devicecount = 0;
  vkEnumeratePhysicalDevices(instance, &devicecount, nullptr);
  if (devicecount == 0)
    throw std::runtime_error("failed to find GPUs with Vulkan support!");

  std::vector<VkPhysicalDevice> devices(devicecount);
  vkEnumeratePhysicalDevices(instance, &devicecount, devices.data());

  // Check if any of the physical devices meet requirements
  for (const auto& device: devices)
  {
    if (vulkan_helpers::IsDeviceSuitable(device, surface))
    {
      physical_device = device;
      break;
    }
  }

  if (physical_device == VK_NULL_HANDLE)
    throw std::runtime_error("Failed to find a suitable GPU!");
}

void VulkanContext::CreateLogicalDevice()
{
  auto queue_families_indices = vulkan_helpers::FindQueueFamilies(physical_device, surface);
  if (!queue_families_indices.IsComplete())
    throw std::runtime_error("Not able to find a suitable queue family");

  float queue_priority = 1.f;
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos{};
  std::set<uint32_t> queue_family_indices = {queue_families_indices.graphics_family.value(),
                                             queue_families_indices.present_family.value()};
  for (auto queue_family_index: queue_family_indices)
  {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures devicefeatures{};

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

  create_info.pEnabledFeatures = &devicefeatures;

  create_info.enabledExtensionCount = static_cast<uint32_t>(kDeviceExtensions.size());
  create_info.ppEnabledExtensionNames = kDeviceExtensions.data();

  if (kEnableValidationLayers)
  {
    create_info.enabledLayerCount = static_cast<uint32_t>(kValidationLayers.size());
    create_info.ppEnabledLayerNames = kValidationLayers.data();
  }
  else
  {
    create_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS)
    throw std::runtime_error("Failed to create logical device!");

  vkGetDeviceQueue(device, queue_families_indices.graphics_family.value(), 0, &graphics_queue);
  vkGetDeviceQueue(device, queue_families_indices.present_family.value(), 0, &present_queue);
}

void VulkanContext::CreateSwapChain(GLFWwindow* window)
{
  SwapChainSupportDetails swap_chain_support = vulkan_helpers::QuerySwapChainSupport(physical_device, surface);

  VkSurfaceFormatKHR surface_format = vulkan_helpers::ChooseSwapSurfaceFormat(swap_chain_support.formats);
  VkPresentModeKHR present_mode = vulkan_helpers::ChooseSwapPresentMode(swap_chain_support.present_modes);
  VkExtent2D extent = vulkan_helpers::ChooseSwapExtent(swap_chain_support.capabilities, window);

  // how many images we would like to have in the swap chain
  // it is recommended to request at least one more image than the minimum
  uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

  // 0 is a special value that means that there is no maximum
  if (swap_chain_support.capabilities.maxImageCount > 0
      && image_count > swap_chain_support.capabilities.maxImageCount)
    image_count = swap_chain_support.capabilities.maxImageCount;

  VkSwapchainCreateInfoKHR create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = surface;
  create_info.minImageCount = image_count;
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;
  // Amount of layers each image consists of.
  // This is always 1 unless you are developing a stereoscopic 3D application
  create_info.imageArrayLayers = 1;
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // how to handle swap chain images that will be used across multiple queue families.
  QueueFamilyIndices indices = vulkan_helpers::FindQueueFamilies(physical_device, surface);
  uint32_t queue_family_indices[] = {indices.graphics_family.value(), indices.present_family.value()};

  if (indices.graphics_family != indices.present_family)
  {
    // Images can be used across multiple queue families without explicit ownership transfers.
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
  }
  else
  {
    // An image is owned by one queue family at a time
    // ownership must be explicitly transferred before using it in another queue family
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;     // Optional
    create_info.pQueueFamilyIndices = nullptr; // Optional
  }

  create_info.preTransform = swap_chain_support.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(device, &create_info, nullptr, &swap_chain) != VK_SUCCESS)
    throw std::runtime_error("failed to create swap chain!");

  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
  swap_chain_images.resize(image_count);
  vkGetSwapchainImagesKHR(device, swap_chain, &image_count, swap_chain_images.data());

  swap_chain_image_format = surface_format.format;
  swap_chain_extent = extent;
}

void VulkanContext::CreateImageViews()
{
  swap_chain_image_views.resize(swap_chain_images.size());
  for (size_t i = 0; i < swap_chain_images.size(); i++)
  {
    VkImageViewCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = swap_chain_images[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = swap_chain_image_format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
    if (vkCreateImageView(device, &create_info, nullptr, &swap_chain_image_views[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create image views!");
  }
}

void VulkanContext::CreateRenderPass()
{
  // single color buffer attachment represented by one of the images from the swap chain
  VkAttachmentDescription color_attachment{};
  color_attachment.format = swap_chain_image_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  // before rendering and after rendering
  // clear the framebuffer to black before drawing a new frame
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  // store operation to see the drawn objects in the screen
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref{};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  // The index of the attachment in this array is directly referenced
  // from the fragment shader with the layout(location = 0) out vec4 outColor directive
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  if (vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
    throw std::runtime_error("failed to create render pass!");
}

void VulkanContext::CreateGraphicsPipeline()
{
  auto vert_shader_code = vulkan_helpers::ReadSource("shaders/vert.spv");
  auto frag_shader_code = vulkan_helpers::ReadSource("shaders/frag.spv");
  if (vert_shader_code.empty() || frag_shader_code.empty())
    throw std::runtime_error("failed to read shaders!");
  VkShaderModule vert_shader_module = vulkan_helpers::CreateShaderModule(vert_shader_code, device);
  VkShaderModule frag_shader_module = vulkan_helpers::CreateShaderModule(frag_shader_code, device);

  VkPipelineShaderStageCreateInfo vert_shader_stage_create_info{};
  vert_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_create_info.module = vert_shader_module;
  // entrypoint
  vert_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo frag_shader_stage_create_info{};
  frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_create_info.module = frag_shader_module;
  frag_shader_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = {vert_shader_stage_create_info, frag_shader_stage_create_info};

  // // Dynamic state
  // std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
  // VkPipelineDynamicStateCreateInfo dynamicState{};
  // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  // dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  // dynamicState.pDynamicStates = dynamicStates.data();

  // Vertex input
  // describes the format of the vertex data that will be passed to the vertex shader
  VkPipelineVertexInputStateCreateInfo vertex_input_info{};
  vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  // Bindings: spacing between data and whether the data is per-vertex or per-instance (see instancing)
  vertex_input_info.vertexBindingDescriptionCount = 0;
  vertex_input_info.pVertexBindingDescriptions = nullptr; // Optional
  // Attribute descriptions: type of the attributes passed to the vertex shader, which binding to load them from and at which offset
  vertex_input_info.vertexAttributeDescriptionCount = 0;
  vertex_input_info.pVertexAttributeDescriptions = nullptr; // Optional

  // Input assembly
  // what kind of geometry will be drawn from the vertices and if primitive restart should be enabled
  VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
  input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  // We intend to draw triangles
  input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_info.primitiveRestartEnable = VK_FALSE;

  // Viewports and scissors
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)swap_chain_extent.width;
  viewport.height = (float)swap_chain_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swap_chain_extent;

  VkPipelineViewportStateCreateInfo viewport_state_info{};
  viewport_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_info.viewportCount = 1;
  viewport_state_info.pViewports = &viewport;
  viewport_state_info.scissorCount = 1;
  viewport_state_info.pScissors = &scissor;

  // Rasterizer
  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // Optional
  rasterizer.depthBiasClamp = 0.0f;          // Optional
  rasterizer.depthBiasSlopeFactor = 0.0f;    // Optional

  // Multisampling
  // one of the ways to perform anti-aliasing
  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;          // Optional
  multisampling.pSampleMask = nullptr;            // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
  multisampling.alphaToOneEnable = VK_FALSE;      // Optional

  // Color blending
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask =
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

  VkPipelineColorBlendStateCreateInfo color_blending{};
  color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending.logicOpEnable = VK_FALSE;
  color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
  color_blending.attachmentCount = 1;
  color_blending.pAttachments = &color_blend_attachment;
  color_blending.blendConstants[0] = 0.0f; // Optional
  color_blending.blendConstants[1] = 0.0f; // Optional
  color_blending.blendConstants[2] = 0.0f; // Optional
  color_blending.blendConstants[3] = 0.0f; // Optional

  VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_create_info.setLayoutCount = 0;            // Optional
  pipeline_layout_create_info.pSetLayouts = nullptr;         // Optional
  pipeline_layout_create_info.pushConstantRangeCount = 0;    // Optional
  pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional

  if (vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS)
    throw std::runtime_error("failed to create pipeline layout!");

  std::vector<VkDynamicState> dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamic_state_info{};
  dynamic_state_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
  dynamic_state_info.pDynamicStates = dynamic_states.data();

  VkGraphicsPipelineCreateInfo pipeline_create_info{};
  pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = shader_stages;
  pipeline_create_info.pVertexInputState = &vertex_input_info;
  pipeline_create_info.pInputAssemblyState = &input_assembly_info;
  pipeline_create_info.pViewportState = &viewport_state_info;
  pipeline_create_info.pRasterizationState = &rasterizer;
  pipeline_create_info.pMultisampleState = &multisampling;
  pipeline_create_info.pDepthStencilState = nullptr; // Optional
  pipeline_create_info.pColorBlendState = &color_blending;
  pipeline_create_info.pDynamicState = &dynamic_state_info;
  pipeline_create_info.layout = pipeline_layout;
  pipeline_create_info.renderPass = render_pass;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_create_info.basePipelineIndex = -1; // Optional

  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &graphics_pipeline)
      != VK_SUCCESS)
    throw std::runtime_error("failed to create graphics pipeline!");

  vkDestroyShaderModule(device, vert_shader_module, nullptr);
  vkDestroyShaderModule(device, frag_shader_module, nullptr);
}

void VulkanContext::CreateFramebuffers()
{
  swap_chain_framebuffers.resize(swap_chain_image_views.size());
  for (size_t i = 0; i < swap_chain_image_views.size(); i++)
  {
    VkImageView attachments[] = {swap_chain_image_views[i]};

    VkFramebufferCreateInfo framebuffer_info{};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = attachments;
    framebuffer_info.width = swap_chain_extent.width;
    framebuffer_info.height = swap_chain_extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &swap_chain_framebuffers[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create framebuffer!");
  }
}

void VulkanContext::CreateCommandPool()
{
  QueueFamilyIndices queue_family_indices = vulkan_helpers::FindQueueFamilies(physical_device, surface);

  VkCommandPoolCreateInfo pool_info{};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();
  if (vkCreateCommandPool(device, &pool_info, nullptr, &command_pool) != VK_SUCCESS)
    throw std::runtime_error("failed to create command pool!");
}

void VulkanContext::CreateCommandBuffers()
{
  command_buffers.resize(kMaxBufferInFlight);
  VkCommandBufferAllocateInfo alloc_info{};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = command_pool;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

  if (vkAllocateCommandBuffers(device, &alloc_info, command_buffers.data()) != VK_SUCCESS)
    throw std::runtime_error("failed to allocate command buffers!");
}

void VulkanContext::CreateSyncObjects()
{
  image_available_semaphores.resize(kMaxBufferInFlight);
  render_finished_semaphores.resize(kMaxBufferInFlight);
  in_flight_fences.resize(kMaxBufferInFlight);

  VkSemaphoreCreateInfo semaphore_info{};
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_info{};
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  // Create the fence in the signaled state,
  // so that the first call to vkWaitForFences() returns immediately
  // since the fence is already signaled
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < kMaxBufferInFlight; i++)
  {
    if (vkCreateSemaphore(device, &semaphore_info, nullptr, &image_available_semaphores[i]) != VK_SUCCESS
        || vkCreateSemaphore(device, &semaphore_info, nullptr, &render_finished_semaphores[i]) != VK_SUCCESS
        || vkCreateFence(device, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS)
      throw std::runtime_error("failed to create synchronization objects for a frame!");
  }
}

// writes the commands we want to execute into a command buffer
void VulkanContext::RecordCommandBuffer(const VkCommandBuffer& command_buffer, uint32_t image_index)
{
  VkCommandBufferBeginInfo begin_info{};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = 0;                  // Optional
  begin_info.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
    throw std::runtime_error("failed to begin recording command buffer!");

  VkRenderPassBeginInfo render_pass_info{};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = render_pass;
  render_pass_info.framebuffer = swap_chain_framebuffers[image_index];
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = swap_chain_extent;

  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;
  vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

  // we did specify viewport and scissor state for this pipeline to be dynamic.
  // So we need to set them in the command buffer before issuing our draw command:
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swap_chain_extent.width);
  viewport.height = static_cast<float>(swap_chain_extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swap_chain_extent;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  vkCmdDraw(command_buffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(command_buffer);

  if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
    throw std::runtime_error("failed to record command buffer!");
}

void VulkanContext::RecreateSwapChain(GLFWwindow* window)
{
  int width = 0, height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  while (width == 0 || height == 0)
  {
    glfwGetFramebufferSize(window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device);
  CleanupSwapChain();
  CreateSwapChain(window);
  CreateImageViews();
  CreateFramebuffers();
}
