#include "mirror.h"

#include "mirror_camera.h"
#include "system.h"
#include "utils.h"

#include <glm/gtx/string_cast.hpp>

Mirror::Mirror()
{
  LoadDataFromFile("data/shadow/square_fb.msh");
  auto normal_dir = GetMeshes().at(0)->GetVertList()->at(0).normal;
  camera_ = std::make_unique<MirrorCamera>(glm::vec3{position_}, glm::vec3{-normal_dir}, glm::vec3{0.f, 1.f, 0.f});
}

void Mirror::LoadDataFromFile(const std::string& filename)
{
  if (filename.ends_with("msh"))
  {
    auto meshes = utils::GetMeshesFromMshFile(filename);
    for (auto mesh: meshes)
      AddMesh(mesh);
  }
}

void Mirror::Update(float delta_time)
{
  auto normal = model_mtx_ * GetMeshes().at(0)->GetVertList()->at(0).normal;
  auto norm = glm::normalize(normal);

  auto reflect =
    glm::reflect(glm::vec3{position_} - glm::vec3{System::GetCamera()->GetPosition()}, glm::vec3{norm});

  Entity::ComputeModelMatrix();
  camera_->SetPosition(position_);
  camera_->SetLookAt(reflect);
  camera_->Update(delta_time);
}

Camera* Mirror::GetCamera()
{
  return camera_.get();
}

void Mirror::SetPosition(const glm::vec4& vect4)
{
  camera_->SetPosition(vect4);
  Entity::SetPosition(vect4);
}