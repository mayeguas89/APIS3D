#pragma once

#define GLAD_ONLY_HEADER
#include "gl1_render.h"

class GL4Render: public GL1Render
{
  /**
   * @brief Vertex Buffer Object
   * 
   */
  struct VBO
  {
    // Identificador del bo del grupo : el grupo es vertice e indice
    GLuint bo_id;

    // Buffer objects para los vertices
    GLuint vbo;

    // Buffer objects para los indices de los vertices
    GLuint idxbo;

    // Buffer objects para las matrices modelo de los objetos
    GLuint vbm;

    // Buffer objects para las alphas de los objetos
    GLuint vba;
  };

public:
  GL4Render(int width, int height);
  ~GL4Render();

  void Init() override;
  void SetupParticle(Emitter* emitter) override;
  void SetupObject(Object* object) override;
  void RemoveObject(Object* object) override;
  void DrawParticles(Emitter* emitter) override;
  void DrawObjects(const std::vector<Object*>* objects) override;
  void DrawObject(Object* object) override;

private:
  // Vamos a acceder a los elementos por ids de cada objeto
  std::unordered_map<int, VBO> buffer_object_list_;
};
