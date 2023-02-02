#pragma once

#define GLAD_ONLY_HEADER
#include "common.h"
#include "input_manager.h"
#include "object.h"
#include "render.h"

class System
{
public:
  System();
  ~System();

  void AddObject(Object* object);

  void Exit();

  void MainLoop();

  static void SetModelMatrix(glm::mat4* model_matrix);
  static glm::mat4* GetModelMatrix();

private:
  void Init();

  static glm::mat4* current_object_model_matrix_;

  Render* render_;
  InputManager* input_manager_;
  std::vector<Object*>* objects_;
  bool end_;
};
