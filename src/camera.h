#pragma once

#include "entity.h"

class Camera: public Entity
{
public:
  enum class ProjectionType
  {
    Orthogonal,
    Perspective
  };

  Camera(ProjectionType type, glm::vec3 position, glm::vec3 look_at, glm::vec3 up);
  virtual void Update(float delta_time) = 0;
  const glm::mat4& GetViewMatrix();
  const glm::mat4& GetProjectionMatrix();
  void SetSpeed(float speed);
  virtual void ComputeProjectionMatrix(ProjectionType type) = 0;

  void SetLookAt(const glm::vec3& look_at)
  {
    look_at_ = look_at;
  }

  const glm::vec3& GetLookAt()
  {
    return look_at_;
  }

protected:
  virtual void ComputeViewMatrix() = 0;
  glm::mat4 view_mtx_;
  glm::mat4 projection_mtx_;
  glm::vec3 up_;
  glm::vec3 look_at_;
  glm::vec3 radius_;
  float angle_;
  float camera_speed_;
  ProjectionType projection_type_;
};
