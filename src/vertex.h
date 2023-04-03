#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"

struct Vertex
{
  glm::vec4 position;
  glm::vec4 color;
  glm::vec4 normal;
  glm::vec4 tangent;
  glm::vec2 texture_coordinates;
  glm::vec4 bone_indices;
  glm::vec4 bone_weight;
};
