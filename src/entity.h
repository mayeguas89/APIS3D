#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"

class Entity
{
public:
  Entity();
  virtual ~Entity() = default;

  glm::vec4 GetPosition();
  void SetPosition(const glm::vec4& vect4);

  glm::vec4 GetRotation();
  void SetRotation(const glm::vec4& vect4);

  glm::vec4 GetScaling();

  glm::mat4& GetModelMatrix();

  void SetModelMatrix(const glm::mat4& matrix);

  virtual void Update(float delta_time) = 0;

  void SetScaling(const glm::vec4& scaling);

  void SetEnabled(bool value)
  {
    enabled_ = value;
  }

  bool IsEnabled()
  {
    return enabled_;
  }

protected:
  glm::vec4 position_;
  glm::vec4 rotation_;
  glm::vec4 scaling_;
  // Matriz modelo
  glm::mat4 model_mtx_;
  bool enabled_ = true;
};
