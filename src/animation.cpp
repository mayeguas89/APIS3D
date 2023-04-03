#include "directional_light.h"
#include "factory_engine.h"
#include "free_camera.h"
#include "general_object.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

  const int kFps = 16;
  GeneralObject dwarf;

  try
  {
    dwarf.LoadDataFromFile("data/dwarf/dwarf.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  dwarf.SetAnimate(true);
  dwarf.SetFrameRate(kFps);

  Light* directional_light = new DirectionalLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                                  .5f,
                                                  .5f,
                                                  .5f,
                                                  glm::vec3(0.f, -10.f, -6.f));

  directional_light->SetPosition({3.f, 10.f, 6.f, 1.f});

  System::SetAmbient(glm::vec3(210.f / 255.f, 188.f / 255.f, 127.f / 255.f));
  System::SetAmbientIntensity(0.5f);
  System::SetupAmbient();

  System::AddLight(directional_light);

  Camera* free_camera = new FreeCamera({30.f, 50.f, 150.f}, {-0.1f, -0.2f, -1.f}, {0.f, 1.f, 0.f}, 5.f);

  System::SetCamera(free_camera);
  System::AddObject(&dwarf);

  try
  {
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();

  delete free_camera;
  delete directional_light;
  return 0;
}