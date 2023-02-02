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

  ~Object();

  Mesh3D* GetMesh();

  void SetMesh(Mesh3D* mesh);

  virtual void LoadDataFromFile(const std::string& filaname) = 0;
  virtual void Update(float delta_time) = 0;

protected:
  Mesh3D* mesh_;
  Type type_;
};
