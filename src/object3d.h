#pragma once

#include "object.h"

class Object3D: public Object
{
public:
  Object3D();
  void LoadDataFromFile(const std::string& filaname) override;
};
