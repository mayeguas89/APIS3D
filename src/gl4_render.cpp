#include "gl4_render.h"

#include "system.h"
#include "vertex.h"

GL4Render::GL4Render(int width, int height): GL1Render{width, height} {}

GL4Render::~GL4Render() {}

void GL4Render::SetupObject(Object* object)
{
  auto* mesh = object->GetMesh();

  VBO vbo;
  glGenVertexArrays(1, &vbo.bo_id);
  glGenBuffers(1, &vbo.vbo);
  glGenBuffers(1, &vbo.idxbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vertex) * mesh->GetVertList()->size(),
               mesh->GetVertList()->data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.idxbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * mesh->GetVertIndexesList()->size(),
               mesh->GetVertIndexesList()->data(),
               GL_STATIC_DRAW);
  buffer_object_list_[mesh->GetMeshId()] = vbo;
}

void GL4Render::RemoveObject(Object* object) {}

void GL4Render::DrawObjects(const std::vector<Object*>* objects)
{
  for (auto i = 0; i < objects->size(); i++)
  {
    System::SetModelMatrix(&(objects->at(i)->GetModelMatrix()));
    auto* mesh = objects->at(i)->GetMesh();
    auto buffer = buffer_object_list_[mesh->GetMeshId()];
    glBindVertexArray(buffer.bo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.idxbo);

    mesh->GetMaterial()->Prepare();
    glDrawElements(GL_TRIANGLES, mesh->GetVertIndexesList()->size(), GL_UNSIGNED_INT, nullptr);
  }
}
