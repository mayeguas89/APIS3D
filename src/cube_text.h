#pragma once
#include "object3d.h"

class CubeText: public Object3D
{
public:
  CubeText();
  ~CubeText();

  void LoadDataFromFile(const std::string& filename) override;

  void Update(float delta_time) override;

private:
  void UpdateModelMatrix();
};
