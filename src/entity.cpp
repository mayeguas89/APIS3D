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

glm::vec4 Entity::GetScaling()
{
  return scaling_;
}

glm::mat4& Entity::GetModelMatrix()
{
  return model_mtx_;
}