#include "time_manager.h"

float TimeManager::oldTime = 0.f;
float TimeManager::deltaTime = 0.f;
float TimeManager::newTime = 0.f;

void TimeManager::Update(double get_time)
{
  newTime = static_cast<float>(get_time);
  deltaTime = newTime - oldTime;
  oldTime = newTime;
}