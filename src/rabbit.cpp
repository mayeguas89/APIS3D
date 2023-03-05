#include "asian_town.h"
#include "camera_keyboard.h"
#include "directional_light.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "orbital_light.h"
#include "point_light.h"
#include "rotate_camera.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

  AsianTown rabbit;
  AsianTown rabbit_two;

  try
  {
    rabbit.LoadDataFromFile("data/bunny.msh");
    rabbit_two.LoadDataFromFile("data/bunny.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  rabbit.SetPosition(glm::vec4(0.5f, 0.f, 0.f, 1.f));
  rabbit.SetScaling(glm::vec4(1.5f, 1.5f, 1.5f, 1.f));
  rabbit.SetRotation(glm::vec4(glm::half_pi<float>(), 0.f, 0.f, 0.f));

  rabbit_two.SetScaling(glm::vec4(1.5f, 1.5f, 1.5f, 1.f));
  rabbit_two.SetRotation(glm::vec4(glm::half_pi<float>(), 0.f, 0.f, 0.f));
  rabbit_two.SetPosition(glm::vec4(-0.5f, 0.f, 0.f, 1.f));

  System::SetAmbient(glm::vec3(210.f / 255.f, 188.f / 255.f, 127.f / 255.f));
  System::SetAmbientIntensity(0.5f);
  System::SetupAmbient();

  Light* point_light = new PointLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                      0.5f,
                                      0.5f,
                                      0.5f,
                                      5.f,
                                      glm::vec3(0.f, -1.f, 0.f));
  System::AddLight(point_light);

  Light* directional_light = new DirectionalLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                                  0.5f,
                                                  0.5f,
                                                  0.5f,
                                                  glm::vec3(0.5f, 0.5f, 0.6f));
  System::AddLight(directional_light);

  Light* orbital_light =
    new OrbitalLight(glm::vec3(0.25f, 0.25f, 0.25f), 0.5f, 0.5f, 0.5f, 10.f, glm::vec3(0.25f, 0.25f, 0.25f));
  System::AddLight(orbital_light);

  Light* flash_light = new FlashLight(glm::vec3(.1f, 1.f, .1f),
                                      0.5f,
                                      0.5f,
                                      0.5f,
                                      10.f,
                                      glm::vec3(.5f, .5f, .5f),
                                      glm::vec3(-0.6f, 0.f, -0.8f),
                                      45.f);

  System::AddLight(flash_light);

  // Camera* rotate_camera = new RotateCamera(Camera::ProjectionType::Perspective,
  //                                          glm::vec3(0.f, 1.f, 1.f),
  //                                          glm::vec3(0.f, -1.f, -1.f),
  //                                          glm::vec3(0.f, 1.f, 0.f),
  //                                          0.1f);

  Camera* rotate_camera = new CameraKeyboard(Camera::ProjectionType::Perspective,
                                             glm::vec3(0.f, 1.f, 1.f),
                                             glm::vec3(0.f, -1.f, -1.f),
                                             glm::vec3(0.f, 1.f, 0.f));

  System::SetCamera(rotate_camera);
  System::AddObject(&rabbit);
  System::AddObject(&rabbit_two);
  System::AddObject(point_light->GetCube());
  System::AddObject(orbital_light->GetCube());
  System::AddObject(flash_light->GetCube());
  System::AddLine(flash_light->GetCube()->GetLine());

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
