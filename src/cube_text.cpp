#include "cube_text.h"

#include "factory_engine.h"

CubeText::CubeText(): Object3D()
{
  mesh_ = new Mesh3D();

  // Front Face
  Vertex v1, v2, v3, v4;
  v1.position = glm::vec4(-0.5f, -0.5f, 0.f, 1.f);
  v1.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v1.texture_coordinates = glm::vec2(0.f, 0.f);
  v2.position = glm::vec4(-0.5f, 0.5f, 0.f, 1.f);
  v2.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v2.texture_coordinates = glm::vec2(0.f, 1.f);
  v3.position = glm::vec4(0.5f, 0.5f, 0.f, 1.f);
  v3.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v3.texture_coordinates = glm::vec2(1.f, 1.f);
  v4.position = glm::vec4(0.5f, -0.5f, 0.f, 1.f);
  v4.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v4.texture_coordinates = glm::vec2(1.f, 0.f);

  mesh_->AddTriangle(v1, 0, v2, 1, v3, 2);
  mesh_->AddTriangle(v1, 0, v3, 2, v4, 3);

  // top Face
  v1.position = glm::vec4(-0.5f, -0.5f, 0.f, 1.f);
  v2.position = glm::vec4(-0.5f, 0.5f, 0.f, 1.f);
  v3.position = glm::vec4(-0.5f, -0.5f, -0.5f, 1.f);
  v4.position = glm::vec4(-0.5f, 0.5f, -0.5f, 1.f);

  mesh_->AddTriangle(v1, 4, v2, 5, v3, 6);
  mesh_->AddTriangle(v1, 4, v3, 6, v4, 7);

  if (auto* material = FactoryEngine::GetNewMaterial(); material)
  {
    auto* texture = FactoryEngine::GetNewTexture();
    texture->Load(
      "C:/Users/mayeg/Documents/U-TAD/Master/ApisTridimensionales/MyAPIS3D/build/Debug/data/front.png");
    texture->Bind();

    std::unordered_map<std::string, RenderType> program_map;
    program_map
      ["C:/Users/mayeg/Documents/U-TAD/Master/ApisTridimensionales/MyAPIS3D/build/Debug/data/program.vertex"] =
        RenderType::Vertex;
    program_map
      ["C:/Users/mayeg/Documents/U-TAD/Master/ApisTridimensionales/MyAPIS3D/build/Debug/data/program.fragment"] =
        RenderType::Fragment;
    material->LoadPrograms(program_map);
    material->SetTexture(texture);
    mesh_->SetMaterial(material);
  }
}

CubeText::~CubeText() {}

void CubeText::LoadDataFromFile(const std::string& filename) {}

void CubeText::Update(float delta_time)
{
  rotation_.y += delta_time * glm::quarter_pi<float>();

  UpdateModelMatrix();
}

void CubeText::UpdateModelMatrix()
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