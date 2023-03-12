#include "shadow_calculation_camera.h"

ShadowCalculationCamera::ShadowCalculationCamera(const glm::vec3& position,
                                                 const glm::vec3& direction,
                                                 const glm::vec3& up):
  Camera(Camera::ProjectionType::Orthogonal, position, direction, up)
{}

void ShadowCalculationCamera::ComputeProjectionMatrix(ProjectionType)
{
  projection_mtx_ = glm::ortho(-10.f, 10.f, -10.f, 10.f, 1.f, 1000.f);
}

void ShadowCalculationCamera::Update(float delta_time)
{
  ComputeProjectionMatrix(projection_type_);
  ComputeViewMatrix();
}

void ShadowCalculationCamera::ComputeViewMatrix()
{
  view_mtx_ = glm::lookAt(glm::vec3(position_), look_at_, up_);
}
