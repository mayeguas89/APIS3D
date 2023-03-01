#include "camera_keyboard.h"
#include "column.h"
#include "factory_engine.h"
#include "light.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System system;

  Column column;
  try
  {
    column.LoadDataFromFile("data/column/column.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  column.SetScaling(glm::vec4(0.01f, 0.01f, 0.01f, 1.f));

  // asian_town.SetScaling(glm::vec4(1.5f, 1.5f, 1.5f, 1.f));

  // CubeText asian_town;

  Light* directional_light = new Light(Light::Type::kDirectional,
                                       glm::vec3(1.f, 1.f, 1.f),
                                       glm::vec3(1.f, 1.f, 1.f),
                                       glm::vec3(1.f, 1.f, 1.f),
                                       0.f);
  directional_light->SetLinearAttenuation(1.f);
  System::SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  System::AddLight(directional_light);

  System::SetCalculateLight(true);

  Camera* camera_keyboard = new CameraKeyboard(Camera::ProjectionType::Perspective,
                                               glm::vec3(0.f, 4.f, 4.f),
                                               glm::vec3(0.f, 0.f, -1.f),
                                               glm::vec3(0.f, 1.f, 0.f));
  system.SetCamera(camera_keyboard);
  system.AddObject(&column);
  system.MainLoop();

  delete camera_keyboard;

  return 0;
}
