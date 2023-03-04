#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "particle.h"

class Emitter
{
public:
  Emitter(const std::string& particle_filename,
          const glm::vec3& initial_particle_position,
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

  std::vector<glm::vec2>& GetParticleVertexTextList()
  {
    return particle_vertex_text_list_;
  }

  std::vector<glm::vec4>& GetParticleVertexPosList()
  {
    return particle_vertex_pos_list_;
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
  std::list<Particle*> particles_;
  float particles_waiting_to_be_created_ = 0.f;
  const std::string particle_filename_;
  const glm::vec3 initial_particle_position_;
  bool autofade_;
  std::vector<glm::mat4> model_matrices_;
  std::vector<glm::vec4> particle_vertex_pos_list_;
  std::vector<glm::vec2> particle_vertex_text_list_;
  glm::vec3* center_position_;
  Particle* model_particle_;
};
