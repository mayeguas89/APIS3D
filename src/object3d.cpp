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

    poses_ = utils::GetArmatureFromMsfFile(filename);
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

void Object3D::Update(float delta_time)
{
  static float time_accum = 0;
  time_accum += delta_time;

  if (poses_ != nullptr)
  {
    if (auto num_frames = poses_->GetLastFrame(); num_frames > 0)
    {
      if (time_accum > abs(frame_rate_))
      {
        // Update frame
        current_frame_ = (current_frame_ + 1) % num_frames;
        CalculateAnimationMatrices();
        System::SetAnimationMatrices(animation_matrices_);
        time_accum -= abs(frame_rate_);
      }
    }
  }
}
