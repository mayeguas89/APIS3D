#pragma once

#include "object.h"

class Billboard: public Object
{
public:
  Billboard(float spin);
  void ComputeModelMatrix() override;
  void Update(float delta_time) override;
  float GetSpin() const;
  void SetSpin(float spin);
  void LoadDataFromFile(const std::string& filename) override;

private:
  float spin_;
};