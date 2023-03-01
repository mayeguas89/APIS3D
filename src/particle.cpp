#include "particle.h"

Particle::Particle(const std::string& filename,
                   const glm::vec3 velocity,
                   float spin,
                   double lifetime,
                   bool autofade):
  Billboard(spin),
  velocity_{velocity},
  lifetime_{lifetime},
  autofade_{autofade}
{
  Billboard::LoadDataFromFile(filename);
}

const glm::vec3& Particle::GetVelocity()
{
  return velocity_;
}
double Particle::GetLifetime()
{
  return lifetime_;
}
bool Particle::GetAutofade()
{
  return autofade_;
}
void Particle::SetVelocity(const glm::vec3& velocity)
{
  velocity_ = velocity;
}
void Particle::SetLifetime(double lifetime)
{
  lifetime_ = lifetime;
}
void Particle::SetAutofade(bool autofade)
{
  autofade_ = autofade;
}