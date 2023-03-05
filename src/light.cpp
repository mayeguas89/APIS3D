#include "light.h"

Light::Light(const glm::vec3& color,
             float ambient_contribution,
             float difuse_contribution,
             float specular_contribution):
  Entity(),
  color_{color},
  ambient_contribution_{ambient_contribution},
  difuse_contribution_{difuse_contribution},
  specular_contribution_{specular_contribution}
{}

const glm::vec3& Light::GetColor() const
{
  return color_;
}

void Light::SetPosition(const glm::vec4& vect4)
{
  if (cube_ == nullptr)
    cube_ = new CubeLight(color_);

  cube_->SetScaling(glm::vec4{kCubeScaling, 1.f});
  Entity::SetPosition(vect4);
  if (cube_)
    cube_->SetPosition(vect4);
}

void Light::SetRotation(const glm::vec4& vect4)
{
  Entity::SetRotation(vect4);
  direction_.x = glm::cos(vect4.x) * glm::cos(vect4.y);
  direction_.y = glm::sin(vect4.y);
  direction_.z = glm::sin(vect4.x) * glm::cos(vect4.y);
  direction_ = glm::normalize(direction_);
}

void Light::SetLightRange(float value)
{
  distance_range_ = value;
  linear_attenuation_ = 4.5f / distance_range_;
  quadratic_attenuation_ = 75.f / (distance_range_ * distance_range_);
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
  if (light_type_ == Type::kFocal && cube_->GetLine() == nullptr)
    cube_->SetLine(direction_);
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
  {
    cube_->SetModelMatrix(glm::scale(model, kCubeScaling));
    if (auto line = cube_->GetLine(); line)
    {
      line->SetModelMatrix(model_mtx_);
    }
  }
}

CubeLight* Light::GetCube()
{
  return cube_;
}

float Light::GetAmbientContribution() const
{
  return ambient_contribution_;
}
void Light::SetAmbientContribution(float ambientContribution)
{
  ambient_contribution_ = ambientContribution;
}
float Light::GetDifuseContribution() const
{
  return difuse_contribution_;
}
void Light::SetDifuseContribution(float difuseContribution)
{
  difuse_contribution_ = difuseContribution;
}
float Light::GetSpecularContribution() const
{
  return specular_contribution_;
}
void Light::SetSpecularContribution(float specularContribution)
{
  specular_contribution_ = specularContribution;
}
