#pragma once

#include "armature.h"
#include "object.h"

class Object3D: public Object
{
public:
  Object3D();
  void LoadDataFromFile(const std::string& filename) override;

private:
  std::unique_ptr<Armature> armature_;
};
