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

  virtual void Update(float delta_time) = 0;

protected:
  glm::vec4 position_;
  glm::vec4 rotation_;
  glm::vec4 scaling_;

  // Matriz modelo
  glm::mat4 model_mtx_;
};
