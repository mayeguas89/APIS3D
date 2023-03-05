#include "camera_keyboard.h"
#include "column.h"
#include "emitter.h"
#include "factory_engine.h"
#include "point_light.h"
#include "rotate_camera.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

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

  Light* point_light =
    new PointLight(glm::vec3(0.75f, 0.75f, 0.75f), 1.f, 1.f, 1.f, 100.f, glm::vec3(0.f, 0.f, -2.f));

  System::SetAmbient(glm::vec3(1.f, 1.f, 1.f));
  System::SetAmbientIntensity(0.7f);
  System::SetupAmbient();
  System::SetClearColor(glm::vec4{0.088f, 0.088f, 0.088f, 1.f});
  System::AddLight(point_light);

  Camera* rotate_camera = new CameraKeyboard(Camera::ProjectionType::Perspective,
                                             glm::vec3(0.f, 10.f, 15.f),
                                             glm::vec3(0.f, 3.f, 0.f),
                                             glm::vec3(0.f, 1.f, 0.f));

  Emitter smoke_emmiter_one("data/column/smoke.msh",
                            glm::vec3{-0.5f, 6.5f, -0.5f},
                            glm::vec3{0.5f, 6.5f, 0.5f},
                            glm::vec2{1.f, 5.f},
                            glm::vec2{0.5f, 1.f},
                            glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                            glm::vec2{30.f, 50.f},
                            glm::vec3{-0.1, 1, -0.1},
                            glm::vec3{0.1, 4, 0.1},
                            true);

  Emitter fire_emitter_one("data/column/flame.msh",
                           glm::vec3{-0.5f, 6.5f, -0.5f},
                           glm::vec3{0.5f, 6.5f, 0.5f},
                           glm::vec2{0.5f, 0.5f},
                           glm::vec2{1.f, 1.f},
                           glm::vec2{0., 0.},
                           glm::vec2{100.f, 150.f},
                           glm::vec3{-1., 5., -1.},
                           glm::vec3{1., 5., 1.},
                           false);

  System::AddEmitter(&fire_emitter_one);
  System::AddEmitter(&smoke_emmiter_one);
  System::SetCamera(rotate_camera);
  System::AddObject(&column);
  System::AddObject(point_light->GetCube());

  try
  {
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();

  delete rotate_camera;
  delete point_light;

  return 0;
}
