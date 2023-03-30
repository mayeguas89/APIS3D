#include "camera_follower.h"

CameraFollower::CameraFollower(std::shared_ptr<Camera> camera): Object3D(), camera_{std::move(camera)} {}

void CameraFollower::Update(float delta_time)
{
  position_ = camera_->GetPosition();
  ComputeModelMatrix();
}
