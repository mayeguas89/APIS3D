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
  // disable handling resized windows
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
  vulkan_context_->Init(window_);
}

void VulkanRender::SetupParticle(Emitter* emitter) {}

void VulkanRender::SetupObject(Object* object) {}

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

void VulkanRender::DrawFrame()
{
  // we want to wait until the previous frame has finished
  // so that the command buffer and semaphores are available to use
  vkWaitForFences(vulkan_context_->device, 1, &vulkan_context_->in_flight_fence, VK_TRUE, UINT64_MAX);

  // manually reset the fence to the unsignaled state
  vkResetFences(vulkan_context_->device, 1, &vulkan_context_->in_flight_fence);

  // Acquiring an image from the swap chain
  uint32_t image_index;
  vkAcquireNextImageKHR(vulkan_context_->device,
                        vulkan_context_->swap_chain,
                        UINT64_MAX,
                        vulkan_context_->image_available_semaphore,
                        VK_NULL_HANDLE,
                        &image_index);

  // Reset the command buffer to make sure it is able to be recorded.
  vkResetCommandBuffer(vulkan_context_->command_buffer, 0);
  vulkan_context_->RecordCommandBuffer(vulkan_context_->command_buffer, image_index);

  // Submitting the command buffer
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {vulkan_context_->image_available_semaphore};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vulkan_context_->command_buffer;
  VkSemaphore signal_semaphores[] = {vulkan_context_->render_finished_semaphore};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(vulkan_context_->graphics_queue, 1, &submit_info, vulkan_context_->in_flight_fence)
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

  vkQueuePresentKHR(vulkan_context_->present_queue, &present_info);
}