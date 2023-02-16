#include "asian_town.h"

AsianTown::AsianTown(): Object3D() {}

void AsianTown::Update(float delta_time)
{
  // La matriz modelo se calcula a partir de la composicion de la transformacion de traslacion, rotacion y escalado

  // La primera siempre se inicializa con la matriz identidad
  glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(position_));

  // Ahora rotamos la matriz en cada uno de los ejes x,y,z el angulo indicado en el vector de rotacion

  // Rotacion eje x
  model = glm::rotate(model, rotation_.x, glm::vec3(1.0f, 0.f, 0.f));
  // Rotacion eje y
  model = glm::rotate(model, rotation_.y, glm::vec3(0.0f, 1.f, 0.f));
  // Rotacion eje z
  model = glm::rotate(model, rotation_.z, glm::vec3(0.0f, 0.f, 1.f));

  // Por ultimo aplicamos el escalado
  model = glm::scale(model, glm::vec3(scaling_));

  // Seteamos la matriz modelo a nuestro modelo
  model_mtx_ = model;
}
