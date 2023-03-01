#pragma once

#include "billboard.h"

class Particle: public Billboard
{
public:
  Particle(const std::string& filename, const glm::vec3 velocity, float spin, double lifetime, bool autofade);

  const glm::vec3& GetVelocity();
  double GetLifetime();
  bool GetAutofade();
  void SetVelocity(const glm::vec3& velocity);
  void SetLifetime(double lifetime);
  void SetAutofade(bool value);
  float GetAlpha();

  void Update(float delta_time) override;

  bool IsDead();

private:
  glm::vec3 velocity_;
  double lifetime_;
  bool autofade_;
  double initial_lifetime_;
};