#include "camera_keyboard.h"
#include "cube_map.h"
#include "factory_engine.h"
#include "flash_light.h"
#include "point_light.h"
#include "rotate_camera.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

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

  Light* point_light = new PointLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                      1.f,
                                      1.f,
                                      1.f,
                                      100.f,
                                      glm::vec3(-1.f, 2.4f, 4.2f));

  System::SetAmbient(glm::vec3(210.f / 255.f, 188.f / 255.f, 127.f / 255.f));
  System::SetAmbientIntensity(0.5f);
  System::SetupAmbient();

  Light* flash_light = new FlashLight(glm::vec3(178.f / 255.f, 205.f / 255.f, 210.f / 255.f),
                                      1.f,
                                      1.f,
                                      1.f,
                                      100.f,
                                      glm::vec3(-0.40f, 0.50f, 2.0f),
                                      glm::vec3(0.07f, -0.20f, -0.98f),
                                      17.f);
  System::AddLight(flash_light);
  System::AddLight(point_light);

  Camera* rotate_camera = new RotateCamera(Camera::ProjectionType::Perspective,
                                           glm::vec3(-0.95f, 0.f, -8.5f),
                                           glm::vec3(0.f, 0.f, -2.f),
                                           glm::vec3(0.f, 1.f, 0.f),
                                           0.08f);

  // Camera* rotate_camera = new CameraKeyboard(Camera::ProjectionType::Perspective,
  //                                            glm::vec3(0.f, 0.f, 3.f),
  //                                            glm::vec3(0.f, 0.f, -1.f),
  //                                            glm::vec3(0.f, 1.f, 0.f));

  System::SetCamera(rotate_camera);
  System::AddObject(&skybox);
  System::AddObject(&teapot);
  System::AddObject(&suzanne);
  System::AddObject(&cube);
  System::AddObject(point_light->GetCube());
  System::AddObject(flash_light->GetCube());

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