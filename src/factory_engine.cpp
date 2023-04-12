#include "factory_engine.h"

#include "gl1_render.h"
#include "gl4_render.h"
#include "gl_texture.h"
#include "glfw_input_manager.h"
#include "glsl_material.h"
#include "vk_material.h"
#include "vk_texture.h"
#include "vulkan_render.h"

FactoryEngine::RenderBackend FactoryEngine::selected_graphics_backend_ = FactoryEngine::RenderBackend::None;
FactoryEngine::InputBackend FactoryEngine::selected_input_backend_ = FactoryEngine::InputBackend::None;

FactoryEngine::RenderBackend FactoryEngine::GetRenderBackend()
{
  return selected_graphics_backend_;
}

FactoryEngine::InputBackend FactoryEngine::GetInputBackend()
{
  return selected_input_backend_;
}

Material* FactoryEngine::GetNewMaterial()
{
  if (selected_graphics_backend_ == FactoryEngine::RenderBackend::GL4Render)
  {
    return new GLSLMaterial();
  }
  else if (selected_graphics_backend_ == FactoryEngine::RenderBackend::VulkanRender)
  {
    return new VkMaterial();
  }
  return nullptr;
}

Texture* FactoryEngine::GetNewTexture()
{
  if (selected_graphics_backend_ == FactoryEngine::RenderBackend::GL4Render
      || selected_graphics_backend_ == FactoryEngine::RenderBackend::GL1Render)
  {
    return new GLTexture();
  }
  else if (selected_graphics_backend_ == FactoryEngine::RenderBackend::VulkanRender)
  {
    return new VkTexture();
  }
  return nullptr;
}

void FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend render_backend)
{
  selected_graphics_backend_ = render_backend;
}

void FactoryEngine::SetInputBackend(FactoryEngine::InputBackend input_backend)
{
  selected_input_backend_ = input_backend;
}

Render* FactoryEngine::GetNewRender()
{
  switch (selected_graphics_backend_)
  {
    case FactoryEngine::RenderBackend::GL1Render:
    {
      GL1Render* render = new GL1Render(600, 480);
      render->SetKeyCallback(GLFWInputManager::KeyManager);
      render->SetCursorPositionCallback(GLFWInputManager::MouseManager);
      render->SetScrollCallback(GLFWInputManager::ScrollManager);
      render->SetMouseButtonCallback(GLFWInputManager::MouseKeyManager);
      return render;
      break;
    }

    case FactoryEngine::RenderBackend::GL4Render:
    {
      GL4Render* render = new GL4Render(600, 480);
      render->SetKeyCallback(GLFWInputManager::KeyManager);
      render->SetCursorPositionCallback(GLFWInputManager::MouseManager);
      render->SetScrollCallback(GLFWInputManager::ScrollManager);
      render->SetMouseButtonCallback(GLFWInputManager::MouseKeyManager);
      return render;
      break;
    }

    case FactoryEngine::RenderBackend::VulkanRender:
    {
      auto render = new VulkanRender(600, 480);
      return render;
      break;
    }

    default:
    {
      return nullptr;
      break;
    }
  }
}

InputManager* FactoryEngine::GetNewInputManager()
{
  switch (selected_input_backend_)
  {
    case FactoryEngine::InputBackend::GLFWInputManager:
    {
      InputManager* inputManager = new GLFWInputManager();
      inputManager->Init();
      return inputManager;
      break;
    }

    default:
    {
      return nullptr;
      break;
    }
  }
}
