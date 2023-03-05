#include "line.h"

#include "factory_engine.h"
Line::Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color): Object()
{
  // Creacion de vertices
  Vertex v0;
  Vertex v1;

  v0.position = glm::vec4(start, 1.f);
  v0.color = glm::vec4(color, 1.f);

  v1.position = glm::vec4(end, 1.f);
  v1.color = glm::vec4(color, 1.f);

  std::unordered_map<std::string, RenderType> program_map;
  program_map["data/line.vertex"] = RenderType::Vertex;
  program_map["data/line.fragment"] = RenderType::Fragment;

  auto material = FactoryEngine::GetNewMaterial();
  material->LoadPrograms(program_map);

  auto mesh = new Mesh3D();
  mesh->AddVertex(v0);
  mesh->AddVertex(v1);
  mesh->AddIndex(0);
  mesh->AddIndex(1);
  mesh->SetMaterial(material);
  AddMesh(mesh);
}

void Line::LoadDataFromFile(const std::string& filename) {}

void Line::Update(float delta_time)
{
  // Entity::ComputeModelMatrix();
}
