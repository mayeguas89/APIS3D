#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "material.h"
#include "vertex.h"

class Mesh3D
{
public:
  Mesh3D();
  ~Mesh3D();

  int GetMeshId();
  void AddVertex(const Vertex& vertex);
  Material* GetMaterial();
  void SetMaterial(Material* material);
  std::vector<Vertex>* GetVertList();
  std::vector<unsigned int>* GetVertIndexesList();
  void AddTriangle(const Vertex& vertex_one,
                   unsigned int index_one,
                   const Vertex& vertex_two,
                   unsigned int index_two,
                   const Vertex& vertex_three,
                   unsigned int index_three);
  void AddIndex(unsigned int index_list);

protected:
  int id_;
  glm::vec3 colorRGB_;
  std::vector<Vertex>* vert_list_;
  std::vector<unsigned int>* indexes_list_;

private:
  static int counter_;
  Material* material_;
};
