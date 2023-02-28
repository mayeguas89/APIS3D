#include "asian_town.h"
#include "camera_keyboard.h"
#include "cube_text.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "light.h"
#include "orbital_light.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System system;

  AsianTown asian_town;
  asian_town.LoadDataFromFile("data/backpack/backpack.obj");
  // asian_town.SetScaling(glm::vec4(1.5f, 1.5f, 1.5f, 1.f));

  // CubeText asian_town;

  Light* directional_light = new Light(Light::Type::kDirectional,
                                       glm::vec3(1.f, 1.f, 1.f),
                                       glm::vec3(1.f, 1.f, 1.f),
                                       glm::vec3(1.f, 1.f, 1.f),
                                       0.f);
  directional_light->SetLinearAttenuation(1.f);

  Light* point_light = new Light(Light::Type::kPoint,
                                 glm::vec3(0.f, .3f, 0.f),
                                 glm::vec3(.5f, .33f, .7f),
                                 glm::vec3(.1f, .1f, 1.f),
                                 0.f);
  point_light->SetLinearAttenuation(.5f);

  Light* orbital_light =
    new OrbitalLight(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));

  orbital_light->SetLinearAttenuation(1.f);
  Light* flash_light =
    new FlashLight(glm::vec3(.5f, .5f, .5f), glm::vec3(-1.f, -1.f, -1.f), glm::vec3(.1f, 1.f, .1f), 60.f);

  System::SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  System::AddLight(directional_light);
  System::AddLight(orbital_light);
  System::AddLight(point_light);
  System::AddLight(flash_light);

  System::SetCalculateLight(true);

  Camera* camera_keyboard = new CameraKeyboard(Camera::ProjectionType::Perspective,
                                               glm::vec3(0.f, 1.f, 1.f),
                                               glm::vec3(0.f, -1.f, -1.f),
                                               glm::vec3(0.f, 1.f, 0.f));
  system.SetCamera(camera_keyboard);
  system.AddObject(&asian_town);
  system.AddObject(std::move(directional_light->GetCube()));
  system.AddObject(std::move(orbital_light->GetCube()));
  system.AddObject(std::move(point_light->GetCube()));
  system.AddObject(std::move(flash_light->GetCube()));
  system.MainLoop();

  delete camera_keyboard;

  return 0;
}
