#pragma once

#include "transformation_in_frame.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


class Bone
{
public:
  inline static const std::string kPositionsKey = "Positions";
  inline static const std::string kRotationsKey = "Rotations";
  inline static const std::string kScalesKey = "Scales";

  Bone(const std::string& name, std::shared_ptr<Bone> parent, int index);
  const std::string& GetName() const
  {
    return name_;
  }

  unsigned int GetParentIndex() const
  {
    return parent_->GetIndex();
  }

  unsigned int GetIndex() const
  {
    return index_;
  }

  const glm::mat4& GetInvPoseMatrix() const
  {
    return inv_pose_matrix_;
  }

  void SetInvPoseMatrix(const glm::mat4& inv_pose_matrix)
  {
    inv_pose_matrix_ = inv_pose_matrix;
  }

  void AddPosition(unsigned int frame, const glm::vec3& position);
  void AddRotation(unsigned int frame, const glm::quat& rotation);
  void AddScale(unsigned int frame, const glm::vec3& scale);

  glm::mat4 CalculateAnimMatrix(unsigned int frame) const;
  glm::vec3 CalculatePosition(unsigned int frame) const;
  glm::quat CalculateRotation(unsigned int frame) const;
  glm::vec3 CalculateScale(unsigned int frame) const;

private:
  std::string name_;
  std::shared_ptr<Bone> parent_;
  int index_;

  glm::mat4 inv_pose_matrix_;

  std::unordered_map<std::string, std::vector<TransformationInFrame>> transformations_map_;
};