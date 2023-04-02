#pragma once

#include "bone.h"

#include <map>
#include <string>

class Armature
{
public:
  Armature() {}

  void AddBone(std::shared_ptr<Bone> bone)
  {
    bone_map_.insert({bone->GetName(), bone});
    bones_.push_back(bone);
  }

  const std::vector<std::shared_ptr<Bone>>& GetBones() const
  {
    return bones_;
  }

  uint16_t GetLastFrame() const
  {
    return last_frame_;
  }

  void SetLastFrame(unsigned int last_frame)
  {
    last_frame_ = last_frame;
  }

  std::shared_ptr<Bone> GetBone(const std::string& name)
  {
    if (auto it = bone_map_.find(name); it != bone_map_.end())
      return it->second;
    return nullptr;
  }

private:
  unsigned int last_frame_;
  std::map<std::string, std::shared_ptr<Bone>> bone_map_;
  std::vector<std::shared_ptr<Bone>> bones_;
};
