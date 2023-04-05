#include "factory_engine.h"
#include "general_object.h"
#include "system.h"
#include "triangle_rot.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::VulkanRender);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  GeneralObject square;
  TriangleRot triangle;
  try
  {
    square.LoadDataFromFile("data/vulkan/square_fb.msh");
    System::Init();
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