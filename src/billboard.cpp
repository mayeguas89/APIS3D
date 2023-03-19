#include "billboard.h"

#include "pugixml.hpp"
#include "system.h"
#include "utils.h"

Billboard::Billboard(float spin): Object(), spin_{spin} {}

void Billboard::Update(float delta_time)
{
  rotation_.z += spin_;
  ComputeModelMatrix();
}

void Billboard::ComputeModelMatrix()
{
  auto camera = System::GetCamera();
  model_mtx_ = camera->GetViewMatrix();
  model_mtx_[0][3] = position_.x;
  model_mtx_[1][3] = position_.y;
  model_mtx_[2][3] = position_.z;
  model_mtx_[3] = glm::vec4(0.f, 0.f, 0.f, 1.0f);

  model_mtx_ = glm::transpose(model_mtx_);
  model_mtx_ = glm::rotate(model_mtx_, rotation_.z, glm::vec3(0.f, 0.f, 1.f));
  model_mtx_ = glm::scale(model_mtx_, glm::vec3(scaling_));
}

float Billboard::GetSpin() const
{
  return spin_;
}

void Billboard::SetSpin(float spin)
{
  spin_ = spin;
}

void Billboard::LoadDataFromFile(const std::string& filename)
{
  if (const auto meshes = System::GetMesh(filename); !meshes.empty())
  {
    for (auto mesh: meshes)
      AddMesh(mesh);
    return;
  }

  if (filename.ends_with("msh"))
  {
    auto meshes = utils::GetMeshesFromMshFile(filename);
    for (auto mesh: meshes)
      AddMesh(mesh);
  }
  else
  {
    auto meshes = utils::GetMeshesFromAssimp(filename);
    for (auto mesh: meshes)
      AddMesh(mesh);
  }
}