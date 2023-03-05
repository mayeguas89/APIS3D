#pragma once
#define GLAD_ONLY_HEADER
#include "common.h"
#include "object.h"

class Line: public Object
{
public:
  Line(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);

  void LoadDataFromFile(const std::string& filename) override;

  void Update(float delta_time) override;
};