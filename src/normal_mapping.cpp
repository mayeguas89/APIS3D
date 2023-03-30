#include "camera_follower.h"
#include "camera_keyboard.h"
#include "cube_map.h"
#include "directional_light.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "orbital_flash_light.h"
#include "orbital_light.h"
#include "point_light.h"
#include "rotate_camera.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

  auto rotate_camera = std::make_shared<RotateCamera>(Camera::ProjectionType::Perspective,
                                                      glm::vec3(0.f, 0.f, -15.f),
                                                      glm::vec3(0.f, 0.f, .1f),
                                                      glm::vec3(0.f, 1.f, 0.f),
                                                      .2f);
  CubeMap skybox;
  CubeMap teapot;
  CubeMap suzanne;
  CubeMap cube;

  try
  {
    skybox.LoadDataFromFile("data/normal_textures/skybox.msh");
    teapot.LoadDataFromFile("data/normal_textures/teapot_reflect.msh");
    suzanne.LoadDataFromFile("data/normal_textures/suzanne_refract.msh");
    cube.LoadDataFromFile("data/normal_textures/normalMapCube.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  skybox.SetScaling(glm::vec4(100.0f, 100.0f, 100.0f, 100.f));
  teapot.SetPosition(glm::vec4(-3.5f, 0.f, 0.f, 1.0f));
  teapot.SetRotation(glm::vec4(0.9f, -0.24f, 0.f, 1.f));
  suzanne.SetPosition(glm::vec4(3.5f, 0.f, 0.f, 1.0f));
  suzanne.SetRotation(glm::vec4(-0.5f, -0.6f, -0.9f, 1.0f));
  cube.SetPosition(glm::vec4(0.f, 0.9f, -1.1f, 1.0f));
  cube.SetRotation(glm::vec4(-0.5f, -0.4f, -0.0f, 1.0f));

  System::SetAmbient(glm::vec3(210.f / 255.f, 188.f / 255.f, 127.f / 255.f));
  System::SetAmbientIntensity(0.5f);
  System::SetupAmbient();

  auto directional_light =
    new DirectionalLight(glm::vec3(1.f, .5f, .8f), .5f, 0.f, 0.f, rotate_camera->GetLookAt());

  auto flash_light = new FlashLight(glm::vec3(1.f, 1.f, 1.f),
                                    0.f,
                                    1.f,
                                    1.f,
                                    50.f,
                                    glm::vec3(0.f, 1.f, -5.f),
                                    glm::normalize(glm::vec3(0.f, -0.02f, .1f)),
                                    12.5f,
                                    15.f);
  auto point_light = new PointLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                    0.f,
                                    1.f,
                                    1.f,
                                    50.f,
                                    glm::vec3(0.f, 3.f, 0.f));
  auto orbital_light = new OrbitalLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                        0.f,
                                        1.f,
                                        1.f,
                                        75.f,
                                        rotate_camera->GetPosition(),
                                        rotate_camera->GetSpeed());

  System::AddLight(directional_light);
  System::AddLight(orbital_light);
  System::AddLight(flash_light);
  System::AddLight(point_light);

  System::SetCamera(rotate_camera.get());
  System::AddObject(&skybox);
  System::AddObject(&teapot);
  System::AddObject(&suzanne);
  System::AddObject(&cube);
  System::AddObject(flash_light->GetCube());
  System::AddObject(point_light->GetCube());
  System::AddObject(orbital_light->GetCube());

  try
  {
    System::MainLoop();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  System::End();

  delete flash_light;
  delete point_light;
  delete directional_light;
  delete orbital_light;

  return 0;
}
