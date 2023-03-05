//
// Created by mayeg on 24/02/2023.
//

#include "cube_light.h"

#include "factory_engine.h"

CubeLight::CubeLight(const glm::vec3& color)
{
  auto material = FactoryEngine::GetNewMaterial();

  std::unordered_map<std::string, RenderType> program_map;
  program_map["data/cube_light.vertex"] = RenderType::Vertex;
  program_map["data/cube_light.fragment"] = RenderType::Fragment;

  material->LoadPrograms(program_map);

  Vertex v0;
  Vertex v1;
  Vertex v2;
  Vertex v3;
  Vertex v4;
  Vertex v5;
  Vertex v6;
  Vertex v7;

  float edge = 0.5f;

  // Front Bottom left
  v0.position = glm::vec4(-edge, -edge, edge, 1.f);
  v0.color = glm::vec4(color, 1.f);

  // Front Top Left
  v1.position = glm::vec4(-edge, edge, edge, 1.f);
  v1.color = glm::vec4(color, 1.f);

  // Front Top Right
  v2.position = glm::vec4(edge, edge, edge, 1.f);
  v2.color = glm::vec4(color, 1.f);

  // Front bottom Right
  v3.position = glm::vec4(edge, -edge, edge, 1.f);
  v3.color = glm::vec4(color, 1.f);

  // Back top Right
  v4.position = glm::vec4(edge, edge, -edge, 1.f);
  v4.color = glm::vec4(color, 1.f);

  // Back bottom Right
  v5.position = glm::vec4(edge, -edge, -edge, 1.f);
  v5.color = glm::vec4(color, 1.f);

  // Back top left
  v6.position = glm::vec4(-edge, edge, -edge, 1.f);
  v6.color = glm::vec4(color, 1.f);

  // Back Bottom left
  v7.position = glm::vec4(-edge, -edge, -edge, 1.f);
  v7.color = glm::vec4(color, 1.f);

  // Front face
  auto mesh = new Mesh3D();
  mesh->SetMaterial(material);
  std::vector<Vertex> vs = {v0, v1, v2, v3, v4, v5, v6, v7};
  for (const auto v: vs)
  {
    mesh->AddVertex(v);
  }
  // clang-format off
  std::vector<int> indexes = {0,1,2,0,2,3,
                              3,2,4,3,4,5,
                              7,6,4,7,4,5,
                              0,7,6,0,6,1,
                              1,6,4,0,2,3,
                              0,7,5,0,5,3};
  // clang-format on
  for (const auto index: indexes)
  {
    mesh->AddIndex(index);
  }
  AddMesh(mesh);
}

void CubeLight::SetColor(const glm::vec3& color)
{
  for (auto mesh: meshes_)
  {
    for (auto& vert: *(mesh->GetVertList()))
    {
      vert.color = glm::vec4(color, 1.f);
    }
  }
}

void CubeLight::LoadDataFromFile(const std::string& filename) {}

void CubeLight::Update(float delta_time)
{
  //  // La matriz modelo se calcula a partir de la composicion de la transformacion de traslacion, rotacion y escalado
  //
  //  // La primera siempre se inicializa con la matriz identidad
  //  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));
  //
  //  // Ahora rotamos la matriz en cada uno de los ejes x,y,z el angulo indicado en el vector de rotacion
  //
  //  // Rotacion eje x
  //  model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.f, 0.f));
  //  // Rotacion eje y
  //  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  //  // Rotacion eje z
  //  model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.f, 1.f));
  //
  //  model = glm::translate(model, glm::vec3(position_));
  //
  //  // Por ultimo aplicamos el escalado
  //  model = glm::scale(model, glm::vec3(scaling_));
  //
  //  // Seteamos la matriz modelo a nuestro modelo
  //  model_mtx_ = model;
}
