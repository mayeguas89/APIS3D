#include "bone.h"

#include <algorithm>
#include <stdexcept>

Bone::Bone(const std::string& name, std::shared_ptr<Bone> parent, int index):
  name_{name},
  parent_{std::move(parent)},
  index_{index}
{
  transformations_map_[kPositionsKey] = {};
  transformations_map_[kRotationsKey] = {};
  transformations_map_[kScalesKey] = {};
}

void Bone::AddPosition(unsigned int frame, const glm::vec3& position)
{
  transformations_map_[kPositionsKey].push_back({frame, position});
}

void Bone::AddRotation(unsigned int frame, const glm::quat& rotation)
{
  transformations_map_[kRotationsKey].push_back({frame, rotation});
}

void Bone::AddScale(unsigned int frame, const glm::vec3& scale)
{
  transformations_map_[kScalesKey].push_back({frame, scale});
}

glm::mat4 Bone::CalculateAnimMatrix(unsigned int frame) const
{
  return glm::mat4{1.f};
}

glm::vec3 Bone::CalculatePosition(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kPositionsKey)->second;
  auto it =
    std::find_if(transformations.begin(), transformations.end(), [&frame](auto& t) { return t.frame == frame; });

  // Si existe el frame
  if (it != transformations.end())
    return std::get<glm::vec3>(it->transformation);

  // Si no existe el frame busco el anterior y el siguiente
  auto it_prev = std::lower_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});
  auto it_next = std::upper_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});

  if (it_next == transformations.end() || it_prev == transformations.end())
    throw std::runtime_error("Frame could not be interpolated");
  auto interpolation_value = (frame - it_prev->frame) / (float)(it_next->frame - it_prev->frame);
  return glm::mix(std::get<glm::vec3>(it_prev->transformation),
                  std::get<glm::vec3>(it_next->transformation),
                  interpolation_value);
}

glm::quat Bone::CalculateRotation(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kRotationsKey)->second;

  if (auto it = std::find_if(transformations.begin(),
                             transformations.end(),
                             [&frame](auto& t) { return t.frame == frame; });
      it != transformations.end())
    return std::get<glm::quat>(it->transformation);

  // Si no existe el frame busco el anterior y el siguiente
  auto it_prev = std::lower_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});
  auto it_next = std::upper_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});

  if (it_next == transformations.end() || it_prev == transformations.end())
    throw std::runtime_error("Frame could not be interpolated");

  auto interpolation_value = (frame - it_prev->frame) / (float)(it_next->frame - it_prev->frame);
  return glm::slerp(std::get<glm::quat>(it_prev->transformation),
                    std::get<glm::quat>(it_next->transformation),
                    interpolation_value);
}

glm::vec3 Bone::CalculateScale(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kScalesKey)->second;
  auto it =
    std::find_if(transformations.begin(), transformations.end(), [&frame](auto& t) { return t.frame == frame; });

  // Si existe el frame
  if (it != transformations.end())
    return std::get<glm::vec3>(it->transformation);

  // Si no existe el frame busco el anterior y el siguiente
  auto it_prev = std::lower_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});
  auto it_next = std::upper_bound(transformations.begin(), transformations.end(), TransformationInFrame{frame});

  if (it_next == transformations.end() || it_prev == transformations.end())
    throw std::runtime_error("Frame could not be interpolated");

  auto interpolation_value = (frame - it_prev->frame) / (float)(it_next->frame - it_prev->frame);
  return glm::mix(std::get<glm::vec3>(it_prev->transformation),
                  std::get<glm::vec3>(it_next->transformation),
                  interpolation_value);
}
