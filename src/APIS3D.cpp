#include "camera_keyboard.h"
#include "cube_map.h"
#include "factory_engine.h"
#include "light.h"
#include "rotate_camera.h"
#include "system.h"

int main(int argc, char const* argv[])
{
  FactoryEngine::SetRenderBackend(FactoryEngine::RenderBackend::GL4Render);
  FactoryEngine::SetInputBackend(FactoryEngine::InputBackend::GLFWInputManager);

  System::Init();

  CubeMap skybox;

  try
  {
    skybox.LoadDataFromFile("data/normal_textures/skybox.msh");
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return -1;
  }
  skybox.SetScaling(glm::vec4(100.0f, 100.0f, 100.0f, 100.f));

  Light* point_light = new Light(Light::Type::kPoint,
                                 glm::vec3(0.f, 15.f, 1.f),
                                 glm::vec3(0.f, -1.f, -1.f),
                                 glm::vec3(0.5f, 0.5f, 0.5f),
                                 0.f,
                                 1.f,
                                 1.f,
                                 1.f,
                                 1.f);

  point_light->SetLinearAttenuation(0.1f);

  System::SetAmbient(glm::vec3(0.1f, 0.1f, 0.1f));

  System::AddLight(point_light);

  Camera* rotate_camera = new RotateCamera(Camera::ProjectionType::Perspective,
                                           glm::vec3(0.f, 0.f, 3.f),
                                           glm::vec3(0.f, 0.f, 0.f),
                                           glm::vec3(0.f, 1.f, 0.f),
                                           0.1f);

  // Camera* rotate_camera = new CameraKeyboard(Camera::ProjectionType::Perspective,
  //                                            glm::vec3(0.f, 0.f, 3.f),
  //                                            glm::vec3(0.f, 0.f, -1.f),
  //                                            glm::vec3(0.f, 1.f, 0.f));

  System::SetCamera(rotate_camera);
  System::AddObject(&skybox);
  // System::AddObject(point_light->GetCube());

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
