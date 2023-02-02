#include "object.h"

Object::Object(): Entity()
{
  mesh_ = nullptr;
  type_ = Type::None;
}

Object::~Object()
{
  delete mesh_;
}

Mesh3D* Object::GetMesh()
{
  return mesh_;
}

void Object::SetMesh(Mesh3D* mesh)
{
  mesh_ = mesh;
}
