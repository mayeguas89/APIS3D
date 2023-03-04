#pragma once

#include "light.h"
#include "object.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class ImguiApp
{
public:
  ImguiApp();
  ~ImguiApp();
  void Init(GLFWwindow* window);
  void Update();

  void AddObject(Object* object);
  void AddLight(Light* light);
  void StartFrame();
  void EndFrame();

private:
  void CameraMenu();
  void ObjectsMenu();
  void LightsMenu();
  void SceneMenu();
  void AddMenu();

  struct ObjectData
  {
    bool enabled = true;
    float translation[4] = {0.0, 0.0, 0.0, 0.0};
    float rotation[4] = {0.0, 0.0, 0.0, 0.0};
    float scale[4] = {0.0, 0.0, 0.0, 0.0};
  };

  struct LightData
  {
    bool enabled = true;
    float position[4] = {0.0, 0.0, 0.0, 0.0};
    float rotation[4] = {0.0, 0.0, 0.0, 0.0};
    ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    float cut_off_angle = 12.5f;
    float linear_attenuation = 0.f;
    float ambient_contribution = 1.f;
    float difuse_contribution = 1.f;
    float specular_contribution = 1.f;
  };

  struct AmbientLightData
  {
    bool enabled = true;
    ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    float intensity = 0.f;
  };

  std::vector<ObjectData> objects_data_;
  std::vector<LightData> light_data_;

  std::vector<Object*> objects_;
  std::vector<Light*> lights_;

  AmbientLightData ambient_light_data_;
};