#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "particle.h"

class Emitter
{
public:
  Emitter(const std::string& particle_filename,
          const glm::vec3& max_initial_position,
          const glm::vec3& min_initial_position,
          const glm::vec2& lifetime_range,
          const glm::vec2& scale_range,
          const glm::vec2& spin_range,
          const glm::vec2& spawn_range,
          const glm::vec3& min_velocity,
          const glm::vec3& max_velocity,
          bool autofade);
  ~Emitter() = default;

  void Update(float delta_time);

  const std::list<Particle*>& GetParticles()
  {
    return particles_;
  }

  std::vector<glm::mat4>& GetModelMatrices()
  {
    return model_matrices_;
  }

  std::vector<float>& GetParticleAlphaList()
  {
    return particle_alpha_list_;
  }

  Particle* GetModelParticle()
  {
    return model_particle_;
  }

private:
  const glm::vec2 lifetime_range_;
  const glm::vec2 scale_range_;
  const glm::vec2 spin_range_;
  const glm::vec2 spawn_range_;
  const glm::vec3 min_velocity_;
  const glm::vec3 max_velocity_;
  const glm::vec3 max_initial_position_;
  const glm::vec3 min_initial_position_;
  std::list<Particle*> particles_;
  float particles_waiting_to_be_created_ = 0.f;
  const std::string particle_filename_;
  bool autofade_;
  std::vector<glm::mat4> model_matrices_;
  std::vector<float> particle_alpha_list_;
  Particle* model_particle_;
};
