#pragma once
#define GLAD_ONLY_HEADER
#include "common.h"
#include "object.h"
#include "render.h"

#include <optional>

class VulkanRender: public Render
{
public:
  VulkanRender(int width, int height);
  ~VulkanRender();

  void Init() override;

  void SetupParticle(Emitter* emitter) override;
  void SetupObject(Object* object) override;
  void SetupLight(Light* light) override;
  void RemoveObject(Object* object) override;
  void DrawObjects(const std::vector<Object*>* objects) override;
  void DrawObject(Object* object) override;
  void DrawParticles(Emitter* emitter) override;
  void DrawLines(const std::vector<Line*>& lines) override;
  bool IsClosed() override;
  void SetSize(int width, int height) override;
  void SetupAmbient() override;
  void SetFrameBuffer(GLuint framebuffer) override;

  void Clear() override;
  void SwapBuffers() override;
  void HideMouse() override;

protected:
  static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
  void DrawFrame();
  void UpdateUniformBuffer(Object* object);
  uint32_t current_frame_ = 0;

  GLFWwindow* window_;
  // -------- Vulkan -----------
  // std::unique_ptr<VulkanContext> vulkan_context_;
};