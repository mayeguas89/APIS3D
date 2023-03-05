#include "cube_text.h"

#include "factory_engine.h"
#define GLAD_ONLY_HEADER
#include "common.h"

CubeText::CubeText(): Object3D()
{
  // Carga de materiales
  auto material_front = FactoryEngine::GetNewMaterial();
  auto material_top = FactoryEngine::GetNewMaterial();

  if (!material_front || !material_top)
    throw std::runtime_error("Selected backend does not support materials");

  auto* texture_front = FactoryEngine::GetNewTexture();
  texture_front->Load({{"data/front.png"}});
  texture_front->Bind();

  auto* texture_top = FactoryEngine::GetNewTexture();
  texture_top->Load({{"data/top.png"}});
  texture_top->Bind();

  std::unordered_map<std::string, RenderType> program_map;
  program_map["data/program.vertex"] = RenderType::Vertex;
  program_map["data/program.fragment"] = RenderType::Fragment;

  material_front->SetShininess(255);
  material_top->SetShininess(255);

  material_front->LoadPrograms(program_map);
  material_front->SetBaseTexture(texture_front);
  material_top->LoadPrograms(program_map);
  material_top->SetBaseTexture(texture_top);

  // Creacion de vertices
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
  v0.color = glm::vec4(1.f, 0.f, 0.f, 0.f);
  v0.texture_coordinates = glm::vec2(0, 0);

  // Front Top Left
  v1.position = glm::vec4(-edge, edge, edge, 1.f);
  v1.color = glm::vec4(0.f, 1.f, 0.f, 0.f);
  v1.texture_coordinates = glm::vec2(0, 1);

  // Front Top Right
  v2.position = glm::vec4(edge, edge, edge, 1.f);
  v2.color = glm::vec4(0.f, 0.f, 1.f, 0.f);
  v2.texture_coordinates = glm::vec2(1, 1);

  // Front bottom Right
  v3.position = glm::vec4(edge, -edge, edge, 1.f);
  v3.color = glm::vec4(0.f, 0.f, 1.f, 0.f);
  v3.texture_coordinates = glm::vec2(1, 0);

  // Back top Right
  v4.position = glm::vec4(edge, edge, -edge, 1.f);
  v4.color = glm::vec4(1.f, 0.f, 1.f, 0.f);
  v4.texture_coordinates = glm::vec2(1, 1);

  // Back bottom Right
  v5.position = glm::vec4(edge, -edge, -edge, 1.f);
  v5.color = glm::vec4(0.f, 1.f, 0.f, 0.f);
  v5.texture_coordinates = glm::vec2(1, 0);

  // Back top left
  v6.position = glm::vec4(-edge, edge, -edge, 1.f);
  v6.color = glm::vec4(1.f, 1.f, 1.f, 0.f);
  v6.texture_coordinates = glm::vec2(0, 1);

  // Back Bottom left
  v7.position = glm::vec4(-edge, -edge, -edge, 1.f);
  v7.color = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
  v7.texture_coordinates = glm::vec2(0, 0);

  // Front face
  auto mesh = new Mesh3D();
  mesh->AddTriangle(v0, 0, v1, 1, v2, 2);
  mesh->AddTriangle(v0, 0, v2, 2, v3, 3);
  mesh->SetMaterial(material_front);
  AddMesh(mesh);

  // Right Face
  mesh = new Mesh3D();
  v3.texture_coordinates = glm::vec2(0, 0);
  v2.texture_coordinates = glm::vec2(0, 1);
  v4.texture_coordinates = glm::vec2(1, 1);
  v5.texture_coordinates = glm::vec2(1, 0);
  mesh->AddTriangle(v3, 0, v2, 1, v4, 2);
  mesh->AddTriangle(v3, 0, v4, 2, v5, 3);
  mesh->SetMaterial(material_front);
  AddMesh(mesh);

  // Back Face
  mesh = new Mesh3D();
  v4.texture_coordinates = glm::vec2(1, 1);
  v5.texture_coordinates = glm::vec2(1, 0);
  v6.texture_coordinates = glm::vec2(0, 1);
  v7.texture_coordinates = glm::vec2(0, 0);
  mesh->AddTriangle(v7, 0, v6, 1, v4, 2);
  mesh->AddTriangle(v7, 0, v4, 2, v5, 3);
  mesh->SetMaterial(material_front);
  AddMesh(mesh);

  // Left Face
  mesh = new Mesh3D();
  v0.texture_coordinates = glm::vec2(1, 0);
  v1.texture_coordinates = glm::vec2(1, 1);
  v6.texture_coordinates = glm::vec2(0, 1);
  v7.texture_coordinates = glm::vec2(0, 0);
  mesh->AddTriangle(v0, 0, v7, 1, v6, 2);
  mesh->AddTriangle(v0, 0, v6, 2, v1, 3);
  mesh->SetMaterial(material_front);
  AddMesh(mesh);

  // Top Face
  mesh = new Mesh3D();
  v1.texture_coordinates = glm::vec2(0, 0);
  v2.texture_coordinates = glm::vec2(1, 0);
  v6.texture_coordinates = glm::vec2(0, 1);
  v4.texture_coordinates = glm::vec2(1, 1);
  mesh->AddTriangle(v1, 0, v6, 1, v4, 2);
  mesh->AddTriangle(v1, 0, v4, 2, v2, 3);
  mesh->SetMaterial(material_top);
  AddMesh(mesh);

  // Bottom Face
  mesh = new Mesh3D();
  v0.texture_coordinates = glm::vec2(0, 0);
  v3.texture_coordinates = glm::vec2(1, 0);
  v5.texture_coordinates = glm::vec2(1, 1);
  v7.texture_coordinates = glm::vec2(0, 1);
  mesh->AddTriangle(v0, 0, v7, 1, v5, 2);
  mesh->AddTriangle(v0, 0, v5, 2, v3, 3);
  mesh->SetMaterial(material_top);
  AddMesh(mesh);
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