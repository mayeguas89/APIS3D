#include "cube_map.h"
#include "directional_light.h"
#include "factory_engine.h"
#include "free_camera.h"
#include "mirror.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

  CubeMap skybox;
  CubeMap scene;
  Mirror* mirror_one = nullptr;
  Mirror* mirror_two = nullptr;
  CubeMap wooden_table;
  CubeMap horse_statue;

  try
  {
    skybox.LoadDataFromFile("data/nissibeach/skybox.msh");
    scene.LoadDataFromFile("data/shadow/scene.msh");
    mirror_one = new Mirror();
    mirror_two = new Mirror();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  skybox.SetScaling(glm::vec4(100.0f, 100.0f, 100.0f, 100.f));
  scene.SetRotation(glm::vec4(0.f, glm::pi<float>(), 0.f, 1.0f));
  mirror_one->SetPosition({-9.5f, 3.f, -1.9f, 1.f});
  mirror_one->SetScaling({10.f, 6.f, 10.f, 1.f});
  mirror_one->SetRotation({0.f, -glm::pi<float>() + glm::quarter_pi<float>(), 0.f, 1.f});
  mirror_two->SetPosition({9.5f, 3.f, -1.9f, 1.f});
  mirror_two->SetScaling({10.f, 6.f, 10.f, 1.f});
  mirror_two->SetRotation({0.f, glm::pi<float>() - glm::quarter_pi<float>(), 0.f, 1.f});

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

  Camera* free_camera = new FreeCamera({-4.9f, 10.4f, 20.4f}, {0.2f, -0.3f, -0.651f}, {0.f, 1.f, 0.f}, 2.f);

  System::SetShadowsCamera(directional_light);
  System::SetShadowsEnabled(true);

  System::SetCamera(free_camera);
  System::AddObject(&skybox);
  System::AddObject(&scene);
  System::AddMirror(mirror_one);
  System::AddMirror(mirror_two);

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
  delete mirror_one;
  delete mirror_two;
  return 0;
}
