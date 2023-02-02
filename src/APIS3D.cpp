#include "factory_engine.h"
#include "system.h"
#include "triangle_rot.h"

int main(int argc, char const* argv[])
{
  // FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL1Render);
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System system;
  TriangleRot triangle;
  system.AddObject(&triangle);
  system.MainLoop();

  return 0;
}
