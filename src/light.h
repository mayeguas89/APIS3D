#pragma once

#include "cube_light.h"
#include "entity.h"

class Light: public Entity
{
public:
  enum class Type
  {
    kDirectional = 0,
    kPoint,
    kFocal
  };

  Light(const glm::vec3& color,
        float ambient_contribution,
        float difuse_contribution,
        float specular_contribution);

  virtual ~Light() = default;
  void SetPosition(const glm::vec4& vect4);
  void SetRotation(const glm::vec4& vect4);

  int GetType()
  {
    return (int)light_type_;
  }

  void SetLightRange(float value);

  float GetLightRange()
  {
    return distance_range_;
  }

  float GetLinearAttenuation()
  {
    return linear_attenuation_;
  }

  void SetLinearAttenuation(float value)
  {
    linear_attenuation_ = value;
  }

  float GetConstantAttenuation()
  {
    return constant_attenuation_;
  }

  void SetConstantAttenuation(float value)
  {
    constant_attenuation_ = value;
  }

  float GetQuadraticAttenuation()
  {
    return quadratic_attenuation_;
  }

  void SetQuadraticAttenuation(float value)
  {
    quadratic_attenuation_ = value;
  }

  float GetCutOff() const
  {
    return cut_off_angle_in_degrees_;
  }

  float GetOuterCutOff() const
  {
    return outer_cut_off_angle_in_degrees_;
  }

  void SetCutOff(float cut_off_angle_in_degrees)
  {
    cut_off_angle_in_degrees_ = cut_off_angle_in_degrees;
  }

  void SetEnabled(bool value)
  {
    Entity::SetEnabled(value);
    if (cube_)
      cube_->SetEnabled(value);
  }

  void SetOuterCutOff(float outer_cut_off_angle_in_degrees)
  {
    outer_cut_off_angle_in_degrees_ = outer_cut_off_angle_in_degrees;
  }

  virtual void Update(float delta_time) override;

  const glm::vec3& GetColor() const;
  void SetColor(const glm::vec3& color);
  const glm::vec3& GetDirection() const;
  void SetDirection(const glm::vec3& direction);

  CubeLight* GetCube();

  float GetAmbientContribution() const;
  void SetAmbientContribution(float ambientContribution);
  float GetDifuseContribution() const;
  void SetDifuseContribution(float difuseContribution);
  float GetSpecularContribution() const;
  void SetSpecularContribution(float specularContribution);

protected:
  inline static const glm::vec3 kCubeScaling = glm::vec3{0.1f, 0.1f, 0.1f};
  bool is_on_;
  Type light_type_;
  glm::vec3 color_;
  glm::vec3 direction_;
  float cut_off_angle_in_degrees_;
  float outer_cut_off_angle_in_degrees_;
  CubeLight* cube_ = nullptr;
  float constant_attenuation_ = 1.f;
  float linear_attenuation_ = 0.f;
  float quadratic_attenuation_ = 0.f;
  float ambient_contribution_ = 1.f;
  float distance_range_ = 0.f;

  float difuse_contribution_ = 1.f;
  float specular_contribution_ = 1.f;
};
