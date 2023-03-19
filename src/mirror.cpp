#include "mirror.h"

#include "mirror_camera.h"
#include "system.h"
#include "utils.h"

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
  Entity::ComputeModelMatrix();
  //   glm::vec3 look_at;
  //   look_at.x = glm::cos(glm::radians(rotation_.x)) * glm::cos(glm::radians(rotation_.y));
  //   look_at.y = glm::sin(glm::radians(rotation_.y));
  //   look_at.z = glm::sin(glm::radians(rotation_.x)) * glm::cos(glm::radians(rotation_.y));
  //   auto position = glm::inverse(model_mtx_) * camera_->GetPosition();
  camera_->SetPosition(position_);
  //   camera_->SetLookAt(look_at);
  camera_->Update(delta_time);
}

Camera* Mirror::GetCamera()
{
  return camera_.get();
}
