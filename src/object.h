#pragma once

#define GLAD_ONLY_HEADER
#include "armature.h"
#include "common.h"
#include "entity.h"
#include "mesh3d.h"

#include <glm/glm.hpp>

#include <vector>

class Object: public Entity
{
public:
  enum Type
  {
    None = -1
  };

  Object();

  ~Object();

  std::vector<Mesh3D*>& GetMeshes();

  void AddMesh(Mesh3D* mesh);

  void SetAlpha(float alpha);

  virtual void LoadDataFromFile(const std::string& filename) = 0;
  virtual void Update(float delta_time) = 0;

  void SetEnabled(bool value)
  {
    Entity::SetEnabled(value);
    float alpha = value ? 1.0f : 0.0f;
    for (auto mesh: meshes_)
    {
      for (auto& v: *(mesh->GetVertList()))
      {
        v.color = glm::vec4(glm::vec3(v.color), alpha);
      }
    }
  }

  void SetAnimate(bool value)
  {
    animate_ = value;
  }

  bool Animate()
  {
    return animate_;
  }

  void SetFrameRate(int fps)
  {
    frame_rate_ = 1 / (float)fps;
  }

  unsigned int GetCurrentFrame() const
  {
    return current_frame_;
  }

  void SetCurrentFrame(unsigned int frame)
  {
    current_frame_ = frame;
  }

  void CalculateAnimationMatrices();

  const std::vector<glm::mat4>& GetAnimationMatrices() const
  {
    return animation_matrices_;
  }

protected:
  std::vector<Mesh3D*> meshes_;
  Type type_;
  std::unique_ptr<Armature> poses_ = nullptr;
  std::vector<glm::mat4> animation_matrices_;
  bool animate_ = false;
  unsigned int current_frame_ = 0;
  float frame_rate_;
};
