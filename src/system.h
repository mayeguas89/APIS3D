#pragma once
#define GLAD_ONLY_HEADER
#include "camera.h"
#include "common.h"
#include "emitter.h"
#include "input_manager.h"
#include "light.h"
#include "object.h"
#include "render.h"

class System
{
public:
  static const int kMaxParticles = 100;
  static void SetupParticle(Emitter* emitter);
  static void AddObject(Object* object);
  static void AddEmitter(Emitter* emitter);
  static void AddLight(Light* light);
  //  void DeleteLight(Light* light);
  static const std::vector<Light*>& GetLights();
  static void SetAmbient(const glm::vec3& value);
  static const glm::vec3& GetAmbient();
  static void SetAmbientIntensity(float value);
  static float GetAmbientIntensity();

  static void Exit();

  static void MainLoop();

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

  static bool GetCalculateLight();
  static void SetCalculateLight(bool calculateLight);
  static void AddMesh(const std::string& filename, Mesh3D* mesh);
  static const std::vector<Mesh3D*>& GetMesh(const std::string& filename);

  static void Init();
  static void End();
  static const glm::vec4& GetClearColor();

  static void SetClearColor(const glm::vec4& color);

private:
  static float near_plane_;
  static float far_plane_;

  static glm::mat4* current_object_model_matrix_;

  static Camera* camera_;

  static Render* render_;
  static InputManager* input_manager_;
  static std::vector<Object*>* objects_;
  static std::vector<Light*> lights_;
  static glm::vec3 ambient_;
  inline static float ambient_intensity_ = 1.f;
  inline static bool calculate_light_ = false;
  inline static std::unordered_map<std::string, std::vector<Mesh3D*>> mesh_map_;
  inline static std::vector<Emitter*> emitters_;

  inline static bool end_ = false;
};
