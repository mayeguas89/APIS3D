#include "asian_town.h"
#include "camera_keyboard.h"
#include "cube_text.h"
#include "factory_engine.h"
#include "light.h"
#include "system.h"
#include "triangle_rot.h"
int main(int argc, char const* argv[])
{
  // FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL1Render);
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System system;

  Light* light = new Light(Light::Type::kDirectional, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
  System::SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  System::AddLight(light);
  System::setCalculateLight(true);
  // TriangleRot triangle;
  // CubeText cube;
  AsianTown asian_town;
  asian_town.LoadDataFromFile("data/bunny.msh");
  Camera* camera_keyboard = new CameraKeyboard(Camera::ProjectionType::Perspective,
                                               glm::vec3(0.f, 1.f, 1.f),
                                               glm::vec3(0.f, -1.f, -1.f),
                                               glm::vec3(0.f, 1.f, 0.f));
  system.SetCamera(camera_keyboard);
  system.AddObject(&asian_town);
  system.MainLoop();

  delete camera_keyboard;

  return 0;
}
