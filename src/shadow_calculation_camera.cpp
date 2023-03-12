#include "shadow_calculation_camera.h"

ShadowCalculationCamera::ShadowCalculationCamera(Light* light):
  Camera(Camera::ProjectionType::Orthogonal,
         glm::vec3{light->GetPosition()},
         light->GetDirection(),
         {0.f, 1.f, 0.f}),
  light_{light}
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
  view_mtx_ = glm::lookAt(glm::vec3(light_->GetPosition()), light_->GetDirection(), up_);
}
