#include "emitter.h"

#include "system.h"
Emitter::Emitter(const std::string& particle_filename,
                 const glm::vec3& initial_particle_position,
                 const glm::vec2& lifetime_range,
                 const glm::vec2& scale_range,
                 const glm::vec2& spin_range,
                 const glm::vec2& spawn_range,
                 const glm::vec3& min_velocity,
                 const glm::vec3& max_velocity,
                 bool autofade):
  particle_filename_{particle_filename},
  initial_particle_position_{initial_particle_position},
  lifetime_range_{lifetime_range},
  scale_range_{scale_range},
  spin_range_{spin_range_},
  spawn_range_{spawn_range},
  min_velocity_{min_velocity},
  max_velocity_{max_velocity},
  autofade_{autofade}
{
  model_particle_ = new Particle(particle_filename_, glm::vec3{0.f}, 0.f, 0.f, false);
  for (auto v: *(model_particle_->GetMeshes()[0]->GetVertList()))
  {
    particle_vertex_pos_list_.push_back(v.position);
    particle_vertex_text_list_.push_back(v.texture_coordinates);
  }
  System::SetupParticle(this);
}

void Emitter::Update(float delta_time)
{
  // New particles this frame
  auto random_spawn_rate{glm::linearRand(spawn_range_.x, spawn_range_.y)};

  auto particles_to_create_base_on_delta_time = random_spawn_rate * delta_time;
  float particles_to_create_this_frame;
  particles_waiting_to_be_created_ =
    std::modf(particles_waiting_to_be_created_ + particles_to_create_base_on_delta_time,
              &particles_to_create_this_frame);

  for (auto i = 0; i < (int)particles_to_create_this_frame; i++)
  {
    auto random_velocity = glm::linearRand(min_velocity_, max_velocity_);
    auto random_lifetime{glm::linearRand(lifetime_range_.x, lifetime_range_.y)};
    auto random_spin{glm::linearRand(spin_range_.x, spin_range_.y)};
    auto random_scale{glm::linearRand(scale_range_.x, scale_range_.y)};

    auto new_particle = new Particle(particle_filename_, random_velocity, random_spin, random_lifetime, autofade_);
    new_particle->SetPosition(glm::vec4{initial_particle_position_, 1.f});
    new_particle->SetScaling({random_scale, random_scale, random_scale, 1.f});
    particles_.push_back(std::move(new_particle));
  }

  auto camera = System::GetCamera();
  std::map<float, Particle*> distance_list;
  for (auto particle: particles_)
  {
    if (!particle->IsDead())
    {
      particle->Update(delta_time);
      auto distance = glm::distance(glm::vec3(particle->GetPosition()), glm::vec3(camera->GetPosition()));
      distance_list[distance] = particle;
    }
    else
      delete particle;
  }
  particles_.clear();
  model_matrices_.clear();

  for (auto item: distance_list)
  {
    particles_.push_back(item.second);
    model_matrices_.push_back(item.second->GetModelMatrix());
  }
}
