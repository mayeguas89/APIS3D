#include "object.h"

Object::Object(): Entity()
{
  type_ = Type::None;
}

std::vector<Mesh3D*>& Object::GetMeshes()
{
  return meshes_;
}

void Object::AddMesh(Mesh3D* mesh)
{
  meshes_.push_back(mesh);
}

void Object::SetAlpha(float alpha)
{
  for (auto mesh: meshes_)
  {
    mesh->GetMaterial()->SetAlpha(alpha);
  }
}