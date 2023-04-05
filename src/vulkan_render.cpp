#include "vulkan_render.h"

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

VulkanRender::VulkanRender(int width, int height):
  Render{width, height},
  vulkan_context_{std::make_unique<VulkanContext>()}
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
  {
    // Error del so de versionado de librerias
    throw std::runtime_error("ERROR AT INIT GLFWINIT\n");
  }
  // not create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  // Create window
  window_ = glfwCreateWindow(width_, height_, "Vulkan", nullptr, nullptr);
}

VulkanRender::~VulkanRender()
{
  vkDeviceWaitIdle(vulkan_context_->device);
  vulkan_context_->ClearInstance();

  // ----- GLFW ----
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void VulkanRender::Init()
{
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, &VulkanRender::FramebufferResizeCallback);
  vulkan_context_->Init(window_);
}

void VulkanRender::SetupParticle(Emitter* emitter) {}

void VulkanRender::SetupObject(Object* object)
{
  for (auto mesh: object->GetMeshes())
  {
    vulkan_context_->CreateVertexBuffer(mesh->GetMeshId(), mesh->GetVertList());
    vulkan_context_->CreateIndexBuffer(mesh->GetMeshId(), mesh->GetVertIndexesList());
  }
}

void VulkanRender::SetupLight(Light* light) {}

void VulkanRender::RemoveObject(Object* object) {}

void VulkanRender::DrawObjects(const std::vector<Object*>* objects)
{
  DrawFrame();
}

void VulkanRender::DrawObject(Object* object) {}

void VulkanRender::DrawParticles(Emitter* emitter) {}

void VulkanRender::DrawLines(const std::vector<Line*>& lines) {}

bool VulkanRender::IsClosed()
{
  return glfwWindowShouldClose(window_);
}

void VulkanRender::SetSize(int width, int height) {}

void VulkanRender::SetupAmbient() {}

void VulkanRender::SetFrameBuffer(GLuint framebuffer) {}

void VulkanRender::Clear() {}

void VulkanRender::SwapBuffers() {}

void VulkanRender::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
  auto instance = reinterpret_cast<VulkanRender*>(glfwGetWindowUserPointer(window));
  instance->vulkan_context_->frame_buffer_resized = true;
}

void VulkanRender::DrawFrame()
{
  // we want to wait until the previous frame has finished
  // so that the command buffer and semaphores are available to use
  vkWaitForFences(vulkan_context_->device,
                  1,
                  &vulkan_context_->in_flight_fences[current_frame_],
                  VK_TRUE,
                  UINT64_MAX);

  // Acquiring an image from the swap chain
  uint32_t image_index;
  auto result = vkAcquireNextImageKHR(vulkan_context_->device,
                                      vulkan_context_->swap_chain,
                                      UINT64_MAX,
                                      vulkan_context_->image_available_semaphores[current_frame_],
                                      VK_NULL_HANDLE,
                                      &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    vulkan_context_->RecreateSwapChain(window_);
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  // manually reset the fence to the unsignaled state
  vkResetFences(vulkan_context_->device, 1, &vulkan_context_->in_flight_fences[current_frame_]);

  // Reset the command buffer to make sure it is able to be recorded.
  vkResetCommandBuffer(vulkan_context_->command_buffers[current_frame_], 0);
  vulkan_context_->RecordCommandBuffer(vulkan_context_->command_buffers[current_frame_], image_index);

  // Submitting the command buffer
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {vulkan_context_->image_available_semaphores[current_frame_]};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vulkan_context_->command_buffers[current_frame_];
  VkSemaphore signal_semaphores[] = {vulkan_context_->render_finished_semaphores[current_frame_]};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(vulkan_context_->graphics_queue,
                    1,
                    &submit_info,
                    vulkan_context_->in_flight_fences[current_frame_])
      != VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swap_chains[] = {vulkan_context_->swap_chain};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;

  result = vkQueuePresentKHR(vulkan_context_->present_queue, &present_info);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkan_context_->frame_buffer_resized)
  {
    vulkan_context_->frame_buffer_resized = false;
    vulkan_context_->RecreateSwapChain(window_);
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }

  current_frame_ = (current_frame_ + 1) % VulkanContext::kMaxBufferInFlight;
}