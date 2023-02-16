#pragma once

#include "object3d.h"

class AsianTown: public Object3D
{
public:
  AsianTown();

  void Update(float delta_time) override;
};