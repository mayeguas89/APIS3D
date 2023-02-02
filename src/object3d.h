#pragma once

#include "object.h"

class Object3D: Object
{
public:
  Object3D();
  void LoadDataFromFile(const std::string& filaname) override;
};
