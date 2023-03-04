#include "column.h"

Column::Column() {}

void Column::Update(float delta_time)
{
  Object3D::ComputeModelMatrix();
}