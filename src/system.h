#pragma once
#define GLAD_ONLY_HEADER
#include "camera.h"
#include "common.h"
#include "emitter.h"
#include "input_manager.h"
#include "light.h"
#include "mirror.h"
#include "object.h"
#include "render.h"

class System
{
public:
  enum class RenderType
  {
    kShadow,
    kColor,
  };
  static const int kMaxParticles = 100;
  static void SetupParticle(Emitter* emitter);
  static void AddLine(Line* line);
  static void AddObject(Object* object);
  static void AddEmitter(Emitter* emitter);
  static void AddLight(Light* light);
  //  void DeleteLight(Light* light);
  static const std::vector<Light*>& GetLights();
  static void SetAmbient(const glm::vec3& value);
  static const glm::vec3& GetAmbient();
  static void SetAmbientIntensity(float value);
  static float GetAmbientIntensity();
  static void SetupAmbient();

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
  static void AddTexture(const std::string& filename, Texture* texture);
  static const std::vector<Mesh3D*>& GetMesh(const std::string& filename);
  static Texture* GetTexture(const std::string& filename);

  static void Init();
  static void End();
  static const glm::vec4& GetClearColor();

  static void SetClearColor(const glm::vec4& color);

  static void SetShadowsCamera(Light* light);
  static Camera* GetShadowsCamera();

  static void SetShadowsEnabled(bool value);
  static bool GetShadowsEnabled();
  static void SetRenderType(RenderType type);
  static RenderType GetRenderType();

  static void AddMirror(Mirror* mirror);
  static const std::vector<Object*>& GetMirrors();

private:
  static float near_plane_;
  static float far_plane_;

  static glm::mat4* current_object_model_matrix_;

  static Camera* camera_;

  static Render* render_;
  static InputManager* input_manager_;
  static std::vector<Object*>* objects_;
  static std::vector<Light*> lights_;
  static std::vector<Line*> lines_;
  static glm::vec3 ambient_;
  inline static float ambient_intensity_ = 1.f;
  inline static bool calculate_light_ = false;
  inline static std::unordered_map<std::string, std::vector<Mesh3D*>> mesh_map_;
  inline static std::unordered_map<std::string, Texture*> texture_map_;
  inline static std::vector<Emitter*> emitters_;
  inline static bool shadows_enabled_ = false;

  inline static bool end_ = false;

  inline static Camera* ortographic_camera_ = nullptr;

  inline static RenderType render_type_ = RenderType::kColor;

  inline static std::vector<Object*> mirrors_;
};
