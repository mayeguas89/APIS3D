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

  Light(Type light_type,
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& color,
        float cut_angle_degrees);

  virtual ~Light() = default;

  void SetPosition(const glm::vec4& vect4);

  int GetType()
  {
    return (int)light_type_;
  }

  float GetLinearAttenuation()
  {
    return linear_attenuation_;
  }

  void SetLinearAttenuation(float value)
  {
    linear_attenuation_ = value;
  }

  float GetCutOff() const
  {
    return cut_off_;
  }

  void SetCutOff(float cut_off)
  {
    cut_off_ = cut_off;
  }

  virtual void Update(float delta_time) override;

  const glm::vec3& GetColor() const;
  void SetColor(const glm::vec3& color);
  const glm::vec3& GetDirection() const;
  void SetDirection(const glm::vec3& direction);

  CubeLight* GetCube();

protected:
  Type light_type_;
  glm::vec3 color_;
  glm::vec3 direction_;
  float cut_off_;
  CubeLight* cube_;
  float linear_attenuation_ = 0.f;
};
