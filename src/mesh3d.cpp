#include "mesh3d.h"

int Mesh3D::counter_ = 0;

Mesh3D::Mesh3D(): id_{counter_}
{
  material_ = nullptr;
  counter_++;
  vert_list_ = new std::vector<Vertex>();
  indexes_list_ = new std::vector<unsigned int>();
  colorRGB_ = glm::vec3{1.f, 1.f, 1.f};
}

Mesh3D::~Mesh3D()
{
  // delete material_;
  delete vert_list_;
  delete indexes_list_;
}

int Mesh3D::GetMeshId()
{
  return id_;
}

void Mesh3D::AddVertex(const Vertex& vertex)
{
  vert_list_->push_back(vertex);
}

Material* Mesh3D::GetMaterial()
{
  return material_;
}

void Mesh3D::SetMaterial(Material* material)
{
  material_ = material;
}

std::vector<Vertex>* Mesh3D::GetVertList()
{
  return vert_list_;
}

std::vector<unsigned int>* Mesh3D::GetVertIndexesList()
{
  return indexes_list_;
}

void Mesh3D::AddTriangle(const Vertex& vertex_one,
                         unsigned int index_one,
                         const Vertex& vertex_two,
                         unsigned int index_two,
                         const Vertex& vertex_three,
                         unsigned int index_three)
{
  if (std::count(indexes_list_->begin(), indexes_list_->end(), index_one) == 0)
    vert_list_->push_back(vertex_one);
  if (std::count(indexes_list_->begin(), indexes_list_->end(), index_two) == 0)
    vert_list_->push_back(vertex_two);
  if (std::count(indexes_list_->begin(), indexes_list_->end(), index_three) == 0)
    vert_list_->push_back(vertex_three);

  indexes_list_->push_back(index_one);
  indexes_list_->push_back(index_two);
  indexes_list_->push_back(index_three);
}

void Mesh3D::AddIndex(unsigned int index)
{
  indexes_list_->push_back(index);
}
