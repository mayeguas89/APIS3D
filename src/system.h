#pragma once

#define GLAD_ONLY_HEADER
#include "camera.h"
#include "common.h"
#include "input_manager.h"
#include "light.h"
#include "object.h"
#include "render.h"

class System
{
public:
  System();
  ~System();

  void AddObject(Object* object);
  static void AddLight(Light* light);
  //  void DeleteLight(Light* light);
  static const std::vector<Light*>& GetLights();
  static void SetAmbient(const glm::vec3& value);
  static const glm::vec3& GetAmbient();

  void Exit();

  void MainLoop();

  static void SetModelMatrix(glm::mat4* model_matrix);
  static glm::mat4* GetModelMatrix();
  static void SetCamera(Camera* camera);
  static Camera* GetCamera();
  static InputManager* GetInputManager();
  static Render* GetRender();
  static float GetAspectRatio();
  static float GetNearPlane();
  static void SetNearPlane(float value);
  static float GetFarPlane();
  static void SetFarPlane(float value);
  static void WindowResizeCallback(GLFWwindow* window, int width, int height);

private:
  void Init();
  static float near_plane_;
  static float far_plane_;

  static glm::mat4* current_object_model_matrix_;

  static Camera* camera_;

  static Render* render_;
  static InputManager* input_manager_;
  static std::vector<Object*>* objects_;
  static std::vector<Light*> lights_;
  static glm::vec3 ambient_;
  inline static bool calculate_light_ = false;

public:
  static bool isCalculateLight();
  static void setCalculateLight(bool calculateLight);

private:
  bool end_;
};
