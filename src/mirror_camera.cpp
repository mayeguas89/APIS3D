#include "mirror_camera.h"

#include "system.h"
MirrorCamera::MirrorCamera(const glm::vec3& position, const glm::vec3& look_at, const glm::vec3& up):
  Camera(Camera::ProjectionType::Perspective, position, look_at, up)
{}

void MirrorCamera::Update(float delta_time)
{
  ComputeProjectionMatrix(projection_type_);
  ComputeViewMatrix();
}

void MirrorCamera::ComputeProjectionMatrix(ProjectionType)
{
  projection_mtx_ =
    glm::perspective(angle_, System::GetAspectRatio(), System::GetNearPlane(), System::GetFarPlane());
}

void MirrorCamera::ComputeViewMatrix()
{
  view_mtx_ = glm::lookAt(glm::vec3(position_), look_at_, up_);
}
