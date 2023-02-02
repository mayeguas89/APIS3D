#include "system.h"

#define GLAD_ONLY_HEADER
#include "common.h"
#include "factory_engine.h"
#include "time_manager.h"

glm::mat4* System::current_object_model_matrix_ = nullptr;

System::System()
{
  Init();
  objects_ = new std::vector<Object*>();
  end_ = false;
}

System::~System()
{
  if (render_)
    delete render_;
  if (input_manager_)
    delete input_manager_;
}

void System::AddObject(Object* object)
{
  render_->SetupObject(object);
  objects_->push_back(object);
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

    // Limpiar buffer color
    render_->Clear();

    // Actualiza Objetos
    for (Object* object: *objects_)
    {
      object->Update(TimeManager::deltaTime);
    }

    // Dibuja objectos
    render_->DrawObjects(objects_);

    // Intercambiar el front y el back buffer
    render_->SwapBuffers();

    // Gestionar eventos
    glfwPollEvents();

    if (input_manager_->IsPressed('E'))
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

void System::Init()
{
  input_manager_ = FactoryEngine::GetNewInputManager();
  input_manager_->Init();

  render_ = FactoryEngine::GetNewRender();
  render_->Init();
}
