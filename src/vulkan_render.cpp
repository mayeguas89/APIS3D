#include "vulkan_render.h"

#include "vk_texture.h"
#include "vulkan_context.h"
#include "vulkan_shader.h"

#include <array>

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

VulkanRender::VulkanRender(int width, int height): Render{width, height}
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
  vkDeviceWaitIdle(VulkanContext::device);
  VulkanContext::ClearInstance();

  // ----- GLFW ----
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void VulkanRender::Init()
{
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, &VulkanRender::FramebufferResizeCallback);
  VulkanContext::Init(window_);
}

void VulkanRender::SetupParticle(Emitter* emitter) {}

void VulkanRender::SetupObject(Object* object)
{
  for (auto mesh: object->GetMeshes())
  {
    VulkanContext::CreateVertexBuffer(mesh->GetMeshId(), mesh->GetVertList());
    VulkanContext::CreateIndexBuffer(mesh->GetMeshId(), mesh->GetVertIndexesList());
  }
  auto material = object->GetMeshes().at(0)->GetMaterial();
  auto vk_shader = reinterpret_cast<VulkanShader*>(material->GetRenderProgram());
  VulkanContext::CreateGraphicsPipeline(vk_shader->GetShaderStages());
  auto vk_texture = reinterpret_cast<VkTexture*>(material->GetBaseTexture());
  VulkanContext::CreateDescriptorSets(vk_texture->GetImageView(), vk_texture->GetImageSampler());
}

void VulkanRender::SetupLight(Light* light) {}

void VulkanRender::RemoveObject(Object* object) {}

void VulkanRender::DrawObjects(const std::vector<Object*>* objects)
{
  for (auto object: *objects)
  {
    UpdateUniformBuffer(object);
    DrawFrame();
  }
}

void VulkanRender::DrawObject(Object* object) {}

void VulkanRender::DrawParticles(Emitter* emitter) {}

void VulkanRender::DrawLines(const std::vector<Line*>& lines) {}

bool VulkanRender::IsClosed()
{
  return glfwWindowShouldClose(window_);
}

void VulkanRender::SetSize(int width, int height)
{
  Render::SetSize(width, height);
}

void VulkanRender::SetupAmbient() {}

void VulkanRender::SetFrameBuffer(GLuint framebuffer) {}

void VulkanRender::Clear() {}

void VulkanRender::SwapBuffers() {}

void VulkanRender::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
  auto instance = reinterpret_cast<VulkanRender*>(glfwGetWindowUserPointer(window));
  instance->SetSize(width, height);
  VulkanContext::frame_buffer_resized = true;
}

void VulkanRender::DrawFrame()
{
  // we want to wait until the previous frame has finished
  // so that the command buffer and semaphores are available to use
  vkWaitForFences(VulkanContext::device, 1, &VulkanContext::in_flight_fences[current_frame_], VK_TRUE, UINT64_MAX);

  // Acquiring an image from the swap chain
  uint32_t image_index;
  auto result = vkAcquireNextImageKHR(VulkanContext::device,
                                      VulkanContext::swap_chain,
                                      UINT64_MAX,
                                      VulkanContext::image_available_semaphores[current_frame_],
                                      VK_NULL_HANDLE,
                                      &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
  {
    VulkanContext::RecreateSwapChain(window_);
    return;
  }
  else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
  {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  // manually reset the fence to the unsignaled state
  vkResetFences(VulkanContext::device, 1, &VulkanContext::in_flight_fences[current_frame_]);

  // Reset the command buffer to make sure it is able to be recorded.
  vkResetCommandBuffer(VulkanContext::command_buffers[current_frame_], 0);
  VulkanContext::RecordCommandBuffer(current_frame_, image_index);

  // Submitting the command buffer
  VkSubmitInfo submit_info{};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {VulkanContext::image_available_semaphores[current_frame_]};
  VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &VulkanContext::command_buffers[current_frame_];
  VkSemaphore signal_semaphores[] = {VulkanContext::render_finished_semaphores[current_frame_]};
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(VulkanContext::graphics_queue,
                    1,
                    &submit_info,
                    VulkanContext::in_flight_fences[current_frame_])
      != VK_SUCCESS)
  {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  VkPresentInfoKHR present_info{};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swap_chains[] = {VulkanContext::swap_chain};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;

  result = vkQueuePresentKHR(VulkanContext::present_queue, &present_info);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || VulkanContext::frame_buffer_resized)
  {
    VulkanContext::frame_buffer_resized = false;
    VulkanContext::RecreateSwapChain(window_);
  }
  else if (result != VK_SUCCESS)
  {
    throw std::runtime_error("failed to present swap chain image!");
  }

  current_frame_ = (current_frame_ + 1) % VulkanContext::kMaxBufferInFlight;
}

void VulkanRender::UpdateUniformBuffer(Object* object)
{
  VulkanContext::UniformBufferObject ubo{};
  ubo.model_matrix = object->GetModelMatrix();
  ubo.view_matrix = glm::lookAt(glm::vec3{2.f, 2.f, 2.f}, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f});
  ubo.projection_matrix = glm::perspective(glm::radians(45.f), width_ / (float)height_, 0.1f, 10.f);
  // GLM was originally designed for OpenGL, where the Y coordinate of the clip coordinates is inverted
  ubo.projection_matrix[1][1] *= -1.f;
  // copy the data in the uniform buffer object
  memcpy(VulkanContext::uniform_resources.buffers_mapped[current_frame_], &ubo, sizeof(ubo));
}
