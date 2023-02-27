#include "entity.h"

Entity::Entity()
{
  position_ = glm::vec4(0.f, 0.f, 0.f, 0.f);
  rotation_ = glm::vec4(0.f, 0.f, 0.f, 0.f);
  scaling_ = glm::vec4(1.f, 1.f, 1.f, 1.f);
}

glm::vec4 Entity::GetPosition()
{
  return position_;
}

glm::vec4 Entity::GetRotation()
{
  return rotation_;
}

void Entity::SetRotation(const glm::vec4& vect4)
{
  rotation_ = vect4;
}

void Entity::SetPosition(const glm::vec4& vect4)
{
  position_ = vect4;
}

glm::vec4 Entity::GetScaling()
{
  return scaling_;
}

glm::mat4& Entity::GetModelMatrix()
{
  return model_mtx_;
}

void Entity::SetModelMatrix(const glm::mat4& matrix)
{
  model_mtx_ = matrix;
}

void Entity::SetScaling(const glm::vec4& scaling)
{
  scaling_ = scaling;
}
