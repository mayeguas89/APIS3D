#pragma once

class TimeManager
{
public:
  static float oldTime;
  static float deltaTime;
  static float newTime;

  static void Update(double get_time);
};