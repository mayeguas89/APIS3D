#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "entity.h"
#include "mesh3d.h"

class Object: public Entity
{
public:
  enum Type
  {
    None = -1
  };

  Object();

  virtual ~Object() = default;

  std::vector<Mesh3D*>& GetMeshes();

  void AddMesh(Mesh3D* mesh);

  void SetAlpha(float alpha);

  virtual void LoadDataFromFile(const std::string& filename) = 0;
  virtual void Update(float delta_time) = 0;

  void SetEnabled(bool value)
  {
    Entity::SetEnabled(value);
    float alpha = value ? 1.0f : 0.0f;
    for (auto mesh: meshes_)
    {
      for (auto& v: *(mesh->GetVertList()))
      {
        v.color = glm::vec4(glm::vec3(v.color), alpha);
      }
    }
  }

protected:
  std::vector<Mesh3D*> meshes_;
  Type type_;
};
