#include "column.h"
#include "emitter.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "light.h"
#include "particle.h"
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

  Light* point_light = new Light(Light::Type::kPoint,
                                 glm::vec3(1.f, 5.f, 1.f),
                                 glm::vec3(-1.f, -1.f, -1.f),
                                 glm::vec3(0.5f, 0.5f, 0.5f),
                                 0.f);

  Light* point_light_two = new Light(Light::Type::kPoint,
                                     glm::vec3(2.5f, 1.f, 2.5f),
                                     glm::vec3(1.f, 1.f, 1.f),
                                     glm::vec3(0.1f, 0.8f, 0.1f),
                                     0.f);
  point_light->SetLinearAttenuation(0.4f);
  point_light_two->SetLinearAttenuation(0.4f);
  System::SetAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
  System::AddLight(point_light);
  System::AddLight(point_light_two);

  Camera* rotate_camera = new RotateCamera(Camera::ProjectionType::Perspective,
                                           glm::vec3(0.f, 5.f, 25.f),
                                           glm::vec3(0.f, 3.f, 0.f),
                                           glm::vec3(0.f, 1.f, 0.f),
                                           glm::quarter_pi<float>());

  Emitter smoke_emmiter_one("data/column/smoke.msh",
                            glm::vec3{0.f, 8.f, 0.f},
                            glm::vec2{1.f, 5.f},
                            glm::vec2{0.5f, 2.f},
                            glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                            glm::vec2{5.f, 10.f},
                            glm::vec3{-0.1, 1, -0.1},
                            glm::vec3{0.1, 4, 0.1},
                            true);

  Emitter smoke_emmiter_two("data/column/smoke.msh",
                            glm::vec3{0.2f, 8.f, 0.f},
                            glm::vec2{1.f, 5.f},
                            glm::vec2{0.5f, 2.f},
                            glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                            glm::vec2{5.f, 10.f},
                            glm::vec3{-0.1, 1, -0.1},
                            glm::vec3{0.1, 4, 0.1},
                            true);

  Emitter smoke_emmiter_three("data/column/smoke.msh",
                              glm::vec3{0.15f, 8.f, 0.f},
                              glm::vec2{1.f, 5.f},
                              glm::vec2{0.5f, 2.f},
                              glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                              glm::vec2{5.f, 10.f},
                              glm::vec3{-0.1, 1, -0.1},
                              glm::vec3{0.1, 4, 0.1},
                              true);

  Emitter smoke_emmiter_four("data/column/smoke.msh",
                             glm::vec3{-0.2f, 8.f, 0.f},
                             glm::vec2{1.f, 5.f},
                             glm::vec2{0.5f, 2.f},
                             glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                             glm::vec2{5.f, 10.f},
                             glm::vec3{-0.1, 1, -0.1},
                             glm::vec3{0.1, 4, 0.1},
                             true);

  Emitter smoke_emmiter_five("data/column/smoke.msh",
                             glm::vec3{-0.15f, 8.f, 0.f},
                             glm::vec2{1.f, 5.f},
                             glm::vec2{0.5f, 2.f},
                             glm::vec2{glm::pi<float>() / 6.f, glm::pi<float>() / 3.f},
                             glm::vec2{5.f, 10.f},
                             glm::vec3{-0.1, 1, -0.1},
                             glm::vec3{0.1, 4, 0.1},
                             true);

  Emitter fire_emitter_one("data/column/flame.msh",
                           glm::vec3{0.f, 6.5f, 0.f},
                           glm::vec2{0.5f, 0.5f},
                           glm::vec2{1.f, 1.f},
                           glm::vec2{0., 0.},
                           glm::vec2{10.f, 25.f},
                           glm::vec3{-1., 5., -1.},
                           glm::vec3{1., 5., 1.},
                           false);

  Emitter fire_emitter_two("data/column/flame.msh",
                           glm::vec3{0.2f, 6.5f, 0.f},
                           glm::vec2{0.5f, 0.5f},
                           glm::vec2{1.f, 1.f},
                           glm::vec2{0., 0.},
                           glm::vec2{10.f, 25.f},
                           glm::vec3{-1., 5., -1.},
                           glm::vec3{1., 5., 1.},
                           false);

  Emitter fire_emitter_three("data/column/flame.msh",
                             glm::vec3{-0.2f, 6.5f, 0.f},
                             glm::vec2{0.5f, 0.5f},
                             glm::vec2{1.f, 1.f},
                             glm::vec2{0., 0.},
                             glm::vec2{10.f, 25.f},
                             glm::vec3{-1., 5., -1.},
                             glm::vec3{1., 5., 1.},
                             false);

  Emitter fire_emitter_four("data/column/flame.msh",
                            glm::vec3{0.15f, 6.5f, 0.f},
                            glm::vec2{0.5f, 0.5f},
                            glm::vec2{1.f, 1.f},
                            glm::vec2{0., 0.},
                            glm::vec2{10.f, 25.f},
                            glm::vec3{-1., 5., -1.},
                            glm::vec3{1., 5., 1.},
                            false);

  Emitter fire_emitter_five("data/column/flame.msh",
                            glm::vec3{-0.15f, 6.5f, 0.f},
                            glm::vec2{0.5f, 0.5f},
                            glm::vec2{1.f, 1.f},
                            glm::vec2{0., 0.},
                            glm::vec2{10.f, 25.f},
                            glm::vec3{-1., 5., -1.},
                            glm::vec3{1., 5., 1.},
                            false);

  System::AddEmitter(&fire_emitter_one);
  // System::AddEmitter(&fire_emitter_two);
  // System::AddEmitter(&fire_emitter_three);
  // System::AddEmitter(&fire_emitter_four);
  // System::AddEmitter(&fire_emitter_five);
  // System::AddEmitter(&smoke_emmiter_one);
  // System::AddEmitter(&smoke_emmiter_two);
  // System::AddEmitter(&smoke_emmiter_three);
  // System::AddEmitter(&smoke_emmiter_four);
  // System::AddEmitter(&smoke_emmiter_five);
  System::SetCamera(rotate_camera);
  System::AddObject(&column);
  System::AddObject(point_light->GetCube());
  System::AddObject(point_light_two->GetCube());

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
