#include "bone.h"

#include <glm/gtx/quaternion.hpp>

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
  // Calcular vectores position rotation scaling en frame
  auto position = CalculatePosition(frame);
  auto rotation = CalculateRotation(frame);
  auto scaling = CalculateScale(frame);

  // Calcular matrices transformation
  auto translation_matrix = glm::translate(glm::mat4{1.f}, position);
  auto roration_matrix = glm::toMat4(rotation);
  auto scale_matrix = glm::scale(glm::mat4{1.f}, scaling);

  return translation_matrix * roration_matrix * scale_matrix;
}

glm::vec3 Bone::CalculatePosition(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kPositionsKey)->second;
  if (transformations.size() == 0)
    return glm::vec3{};

  return std::get<glm::vec3>(CalculateInterpolation(transformations, frame, linear_interpolation_));
}

glm::quat Bone::CalculateRotation(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kRotationsKey)->second;
  if (transformations.size() == 0)
    return glm::quat{};
  return std::get<glm::quat>(CalculateInterpolation(transformations, frame, spherical_interpolation_));
}

glm::vec3 Bone::CalculateScale(unsigned int frame) const
{
  auto transformations = transformations_map_.find(kScalesKey)->second;
  if (transformations.size() == 0)
    return glm::vec3{};
  return std::get<glm::vec3>(CalculateInterpolation(transformations, frame, linear_interpolation_));
}

Transformation Bone::LinearInterpolation(Transformation t1, Transformation t2, float rate)
{
  return glm::mix(std::get<glm::vec3>(t1), std::get<glm::vec3>(t1), rate);
}

Transformation Bone::SphericalInterpolation(Transformation t1, Transformation t2, float rate)
{
  return glm::slerp(std::get<glm::quat>(t1), std::get<glm::quat>(t2), rate);
}

Transformation Bone::CalculateInterpolation(
  const std::vector<TransformationInFrame>& transformations,
  unsigned int frame,
  std::function<Transformation(Transformation, Transformation, float)> interpolation_function) const
{
  auto it_next = transformations.begin();
  auto it_prev = transformations.begin();

  while (it_next != transformations.end() && it_next->frame < frame)
  {
    it_prev = it_next;
    it_next++;
  }

  if (it_next == transformations.end())
    return it_prev->transformation;

  if (it_next->frame == frame)
    return it_next->transformation;

  auto interpolation_value = (frame - (it_prev)->frame) / (float)(it_next->frame - it_prev->frame);

  return interpolation_function(it_prev->transformation, it_next->transformation, interpolation_value);
}
