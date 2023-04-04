#include "factory_engine.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::VulkanRender);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  try
  {
    System::Init();
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();
  return 0;
}