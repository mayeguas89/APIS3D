#pragma once

#include "input_manager.h"
#include "material.h"
#include "render.h"

class FactoryEngine
{
public:
  enum class RenderBackend
  {
    None = -1,
    GL1Render = 0,
    GL4Render = 1,
  };

  enum class InputBackend
  {
    None = -1,
    GLFWInputManager = 0,
  };

  static RenderBackend GetRenderBackend();
  static InputBackend GetInputBackend();

  static void SetRenderBackend(RenderBackend render_backend);

  static void SetInputBackend(InputBackend input_backend);

  static Render* GetNewRender();

  static InputManager* GetNewInputManager();

  static Material* GetNewMaterial();

private:
  static RenderBackend selected_graphics_backend_;
  static InputBackend selected_input_backend_;

  FactoryEngine() = delete;
};