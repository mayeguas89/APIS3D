#pragma once

#include "texture.h"

class GLTexture: public Texture
{
public:
  GLTexture();
  ~GLTexture();
  bool Load(const std::string& filename) override;
  void Bind() override;
};
