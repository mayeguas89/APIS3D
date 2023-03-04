#pragma once
#include "object3d.h"
class CubeLight: public Object3D
{
public:
  CubeLight(const glm::vec3& color);
  void SetColor(const glm::vec3& color);
  void LoadDataFromFile(const std::string& filename) override;
  void Update(float delta_time) override;
};
