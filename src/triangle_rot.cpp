#include "triangle_rot.h"

#define GLAD_ONLY_HEADER
#include "common.h"
#include "factory_engine.h"

TriangleRot::TriangleRot(): Object()
{
  mesh_ = new Mesh3D();

  Vertex v1;
  Vertex v2;
  Vertex v3;

  v1.position = glm::vec4(-0.5f, -0.5f, 0.f, 1.f);
  v1.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v2.position = glm::vec4(0.f, 0.5f, 0.f, 1.f);
  v2.color = glm::vec4(0.f, 1.f, 0.f, 0.f);
  v3.position = glm::vec4(0.5f, -0.5f, 0.f, 1.f);
  v3.color = glm::vec4(0.f, 0.f, 1.f, 0.f);

  if (auto* material = FactoryEngine::GetNewMaterial(); material)
  {
    std::unordered_map<std::string, RenderType> program_map;
    program_map["data/program.vertex"] = RenderType::Vertex;
    program_map["data/program.fragment"] = RenderType::Fragment;
    material->LoadPrograms(program_map);

    mesh_->SetMaterial(material);
    mesh_->AddTriangle(v1, 0, v2, 1, v3, 2);
  }
  else
  {
    mesh_->AddVertex(v1);
    mesh_->AddVertex(v2);
    mesh_->AddVertex(v3);
  }
}

void TriangleRot::LoadDataFromFile(const std::string& filaname) {}

void TriangleRot::Update(float delta_time)
{
  rotation_.y += delta_time * glm::quarter_pi<float>();

  UpdateModelMatrix();
}

void TriangleRot::UpdateModelMatrix()
{
  // La matriz modelo se calcula a partir de la composicion de la transformacion de traslacion, rotacion y escalado

  // La primera siempre se inicializa con la matriz identidad
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  // Ahora rotamos la matriz en cada uno de los ejes x,y,z el angulo indicado en el vector de rotacion

  // Rotacion eje x
  model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.f, 0.f));
  // Rotacion eje y
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  // Rotacion eje z
  model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.f, 1.f));

  // Por ultimo aplicamos el escalado
  model = glm::scale(model, glm::vec3(scaling_));

  // Seteamos la matriz modelo a nuestro modelo
  model_mtx_ = model;
}