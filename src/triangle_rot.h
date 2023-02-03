#pragma once

#include "object.h"

class TriangleRot: public Object
{
public:
  TriangleRot();

  void LoadDataFromFile(const std::string& filename) override;

  void Update(float delta_time) override;

private:
  void UpdateModelMatrix();
};