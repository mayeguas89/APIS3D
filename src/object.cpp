#include "object.h"

Object::Object(): Entity(), shininess_{0}
{
  type_ = Type::None;
}

std::vector<Mesh3D*>& Object::GetMeshes()
{
  return meshes_;
}

int Object::GetShininess()
{
  return shininess_;
}

void Object::SetShininess(int value)
{
  shininess_ = value;
}

void Object::AddMesh(Mesh3D* mesh)
{
  meshes_.push_back(mesh);
}
