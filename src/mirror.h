#pragma once

#include "camera.h"
#include "object.h"

class Mirror: public Object
{
public:
  Mirror();
  void LoadDataFromFile(const std::string& filename) override;
  void Update(float delta_time) override;
  Camera* GetCamera();
  void SetPosition(const glm::vec4& vect4);

private:
  std::unique_ptr<Camera> camera_;
};
