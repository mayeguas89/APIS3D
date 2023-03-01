#pragma once

#include "object3d.h"

class Column: public Object3D
{
public:
  Column();

  void Update(float delta_time) override;
};