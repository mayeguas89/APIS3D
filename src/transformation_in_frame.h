#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <variant>

using Transformation = std::variant<std::monostate, glm::vec3, glm::quat>;

struct TransformationInFrame
{
  TransformationInFrame(unsigned int p_frame): frame{p_frame} {}
  TransformationInFrame(unsigned int p_frame, const Transformation& p_transformation):
    frame{p_frame},
    transformation{p_transformation}
  {}

  unsigned int frame;
  Transformation transformation;
};

struct LowerBound
{
  bool operator()(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
  {
    return rhs.frame > lhs.frame;
  }
};

struct UpperBound
{
  bool operator()(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
  {
    return rhs.frame < lhs.frame;
  }
};

bool operator<(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
{
  return rhs.frame < lhs.frame;
}

bool operator>(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
{
  return rhs.frame > lhs.frame;
}

bool operator<=(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
{
  return rhs.frame <= lhs.frame;
}

bool operator>=(const TransformationInFrame& rhs, const TransformationInFrame& lhs)
{
  return rhs.frame >= lhs.frame;
}