#include "object3d.h"

#include "factory_engine.h"
#include "pugixml.hpp"
#include "system.h"
#include "utils.h"
#ifdef ASSIMP_LOAD_ENABLE
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#endif
#include <iostream>

Object3D::Object3D(): Object() {}

void Object3D::LoadDataFromFile(const std::string& filename)
{
  if (const auto meshes = System::GetMesh(filename); !meshes.empty())
  {
    for (auto mesh: meshes)
      AddMesh(mesh);
    return;
  }

  if (filename.ends_with("msh"))
  {
    auto meshes = utils::GetMeshesFromMshFile(filename);
    for (auto mesh: meshes)
      AddMesh(mesh);
  }
#ifdef ASSIMP_LOAD_ENABLE
  else
  {
    auto meshes = utils::GetMeshesFromAssimp(filename);
    for (auto mesh: meshes)
      AddMesh(mesh);
  }
#endif
}
