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
};
