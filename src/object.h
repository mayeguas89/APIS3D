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

  int GetShininess();
  void SetShininess(int value);

  void AddMesh(Mesh3D* mesh);

  virtual void LoadDataFromFile(const std::string& filename) = 0;
  virtual void Update(float delta_time) = 0;

protected:
  std::vector<Mesh3D*> meshes_;
  Type type_;
  int shininess_ = 0;
};
