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

void Object::CalculateAnimationMatrices()
{
  animation_matrices_.clear();
  auto bones = poses_->GetBones();
  for (auto bone: bones)
  {
    auto bone_anim_matrix = bone->CalculateAnimMatrix(current_frame_);
    if (auto parent_bone_index = bone->GetParentIndex(); parent_bone_index.has_value())
    {
      auto parent_anim_matrix = animation_matrices_.at((size_t)(parent_bone_index.value()));
      bone_anim_matrix = parent_anim_matrix * bone_anim_matrix;
    }
    animation_matrices_.push_back(bone_anim_matrix);
  }

  for (size_t i = 0; i < bones.size(); i++)
  {
    animation_matrices_[i] *= bones.at(i)->GetInvPoseMatrix();
  }
}