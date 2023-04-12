#include "factory_engine.h"
#include "object_rotate.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::VulkanRender);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  try
  {
    System::Init();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }

  try
  {
    ObjectRotate square;
    square.LoadDataFromFile("data/vulkan/square_fb.msh");
    System::AddObject(&square);
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();
  return 0;
}