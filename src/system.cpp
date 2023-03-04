#include "system.h"

#include "factory_engine.h"
#include "time_manager.h"

glm::mat4* System::current_object_model_matrix_ = nullptr;
Camera* System::camera_ = nullptr;
Render* System::render_ = nullptr;
InputManager* System::input_manager_ = nullptr;
float System::far_plane_ = 100.f;
float System::near_plane_ = 0.1f;
std::vector<Object*>* System::objects_ = nullptr;
std::vector<Light*> System::lights_;
glm::vec3 System::ambient_ = glm::vec3(1.f);

void System::End()
{
  if (render_)
    delete render_;
  if (input_manager_)
    delete input_manager_;
}

void System::SetupParticle(Emitter* emitter)
{
  render_->SetupParticle(emitter);
}

void System::AddObject(Object* object)
{
  if (object == nullptr)
    return;
  render_->SetupObject(object);
  objects_->push_back(std::move(object));
}

void System::AddEmitter(Emitter* emitter)
{
  emitters_.push_back(std::move(emitter));
}

void System::Exit()
{
  end_ = true;
}

void System::MainLoop()
{
  while (!end_)
  {
    // Calcula delta_time
    TimeManager::Update(glfwGetTime());

    // Gestionar eventos
    glfwPollEvents();

    // Limpiar buffer color
    render_->Clear();

    // Actualizar la camara
    camera_->Update(TimeManager::deltaTime);

    for (auto light: lights_)
    {
      light->Update(TimeManager::deltaTime);
    }

    // Actualiza Objetos
    for (Object* object: *objects_)
      object->Update(TimeManager::deltaTime);

    // Dibuja objectos
    render_->DrawObjects(objects_);

    // Ordena particular en los emisores
    for (auto emitter: emitters_)
      emitter->Update(TimeManager::deltaTime);

    // Pinta particular del emisor
    for (auto emitter: emitters_)
    {
      render_->DrawParticles(emitter);
    }

    // Intercambiar el front y el back buffer
    render_->SwapBuffers();

    if (input_manager_->IsPressed('X'))
    {
      end_ = true;
    }
    end_ |= render_->IsClosed();
  }
}

void System::SetModelMatrix(glm::mat4* model_matrix)
{
  current_object_model_matrix_ = model_matrix;
}

glm::mat4* System::GetModelMatrix()
{
  return current_object_model_matrix_;
}

void System::SetCamera(Camera* camera)
{
  if (camera != nullptr)
    camera_ = camera;
}

Camera* System::GetCamera()
{
  return camera_;
}

InputManager* System::GetInputManager()
{
  return input_manager_;
}

Render* System::GetRender()
{
  return render_;
}

float System::GetAspectRatio()
{
  return render_->GetWidth() / (float)(render_->GetHeight());
}

float System::GetFarPlane()
{
  return far_plane_;
}

void System::SetFarPlane(float value)
{
  far_plane_ = value;
}

void System::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
  render_->SetSize(width, height);
  System::GetCamera()->ComputeProjectionMatrix(Camera::ProjectionType::Perspective);
}

float System::GetNearPlane()
{
  return near_plane_;
}

void System::SetNearPlane(float value)
{
  near_plane_ = value;
}

void System::Init()
{
  objects_ = new std::vector<Object*>();

  input_manager_ = FactoryEngine::GetNewInputManager();
  input_manager_->Init();

  render_ = FactoryEngine::GetNewRender();
  render_->Init();
}

void System::SetAmbient(const glm::vec3& value)
{
  ambient_ = value;
}

void System::AddLight(Light* light)
{
  lights_.push_back(std::move(light));
  render_->SetupLight(light);
}

const std::vector<Light*>& System::GetLights()
{
  return lights_;
}

const glm::vec3& System::GetAmbient()
{
  return ambient_;
}

void System::SetAmbientIntensity(float value)
{
  ambient_intensity_ = value;
}

float System::GetAmbientIntensity()
{
  return ambient_intensity_;
}

const glm::vec4& System::GetClearColor()
{
  return render_->GetClearColor();
}

void System::SetClearColor(const glm::vec4& color)
{
  render_->SetClearColor(color);
}

void System::AddMesh(const std::string& filename, Mesh3D* mesh)
{
  if (auto it = mesh_map_.find(filename); it == mesh_map_.end())
    mesh_map_[filename] = {{std::move(mesh)}};
  else
    mesh_map_[filename].push_back(std::move(mesh));
}

const std::vector<Mesh3D*>& System::GetMesh(const std::string& filename)
{
  if (auto it = mesh_map_.find(filename); it != mesh_map_.end())
    return it->second;
  return {};
}
