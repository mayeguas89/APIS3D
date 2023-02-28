#include "light.h"

Light::Light(Light::Type light_type,
             const glm::vec3& position,
             const glm::vec3& direction,
             const glm::vec3& color,
             float cut_angle_degrees):
  Entity(),
  light_type_{light_type},
  color_{color},
  direction_{direction},
  cut_off_{glm::cos(glm::radians(cut_angle_degrees))}
{
  cube_ = nullptr;
  position_ = glm::vec4(position, 1.f);
  if (light_type_ != Type::kDirectional)
  {
    cube_ = new CubeLight(color_);
    cube_->SetPosition(position_);
    cube_->SetScaling(glm::vec4(0.1f, 0.1f, 0.1f, 1.f));
  }
}
const glm::vec3& Light::GetColor() const
{
  return color_;
}

void Light::SetPosition(const glm::vec4& vect4)
{
  Entity::SetPosition(vect4);
  if (cube_)
    cube_->SetPosition(vect4);
}

void Light::SetColor(const glm::vec3& color)
{
  color_ = color;
  if (cube_)
    cube_->SetColor(color);
}

const glm::vec3& Light::GetDirection() const
{
  return direction_;
}

void Light::SetDirection(const glm::vec3& direction)
{
  direction_ = direction;
}

void Light::Update(float delta_time)
{
  // La matriz modelo se calcula a partir de la composicion de la transformacion de traslacion, rotacion y escalado

  // La primera siempre se inicializa con la matriz identidad
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  // Ahora rotamos la matriz en cada uno de los ejes x,y,z el angulo indicado en el vector de rotacion

  // Rotacion eje x
  model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.f, 0.f));
  // Rotacion eje y
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  // Rotacion eje z
  model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.f, 1.f));

  // Por ultimo aplicamos el escalado
  model_mtx_ = glm::scale(model, glm::vec3(scaling_));

  // Seteamos la matriz modelo a nuestro modelo
  if (cube_)
    cube_->SetModelMatrix(glm::scale(model, glm::vec3(.2f, .2f, .2f)));
}

CubeLight* Light::GetCube()
{
  return cube_;
}
