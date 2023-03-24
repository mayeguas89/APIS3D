#include "column.h"
#include "directional_light.h"
#include "emitter.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "particle.h"
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

  // auto point_light_top = new PointLight({1.f, 1.f, 1.f}, 0.05f, 0.8f, 1.f, 7.f, glm::vec3(0.f, 5.f, 0.5f));
  auto flash_light_bottom = new FlashLight({1.f, 1.f, 1.f},
                                           0.f,
                                           1.f,
                                           1.f,
                                           25.f,
                                           glm::vec3(0.f, -3.f, 2.f),
                                           glm::vec3(0.f, 1.f, -1.f),
                                           12.5f,
                                           15.f);
  System::SetAmbient(glm::vec3(1.f, 1.f, 1.f));
  System::SetAmbientIntensity(0.15f);

  // System::AddLight(point_light_top);
  System::AddLight(flash_light_bottom);

  Camera* rotate_camera = new RotateCamera(Camera::ProjectionType::Perspective,
                                           glm::vec3(0.f, 10.f, 25.f),
                                           glm::vec3(0.f, 3.f, 0.f),
                                           glm::vec3(0.f, 1.f, 0.f),
                                           glm::quarter_pi<float>() / 2.f);

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
  // System::AddObject(point_light_top->GetCube());
  System::AddObject(flash_light_bottom->GetCube());

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
  delete flash_light_bottom;
  // delete point_light_top;

  return 0;
}
