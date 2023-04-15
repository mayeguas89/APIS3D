#include "camera_keyboard.h"
#include "common.h"
#include "factory_engine.h"
#include "general_object.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();
  System::HideMouse();

  GeneralObject asian_town;

  try
  {
    asian_town.LoadDataFromFile("data/asian_town.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  auto camera = std::make_shared<CameraKeyboard>(Camera::ProjectionType::Perspective,
                                                 glm::vec3(0.f, 0.02f, 0.f),
                                                 glm::vec3(0.f, 1.f, -0.01f),
                                                 glm::vec3(0.f, 1.f, 0.f));
  camera->SetSpeed(0.15f);
  System::SetCamera(camera.get());
  System::SetNearPlane(0.01f);
  System::AddObject(&asian_town);

  try
  {
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();

  return 0;
}
