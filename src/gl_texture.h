#pragma once

#include "texture.h"

class GLTexture: public Texture
{
public:
  GLTexture();
  ~GLTexture();
  bool Load(const std::vector<std::string>& filenames, Type type = Type::kColor2D) override;
  void Bind(unsigned int index = 0) override;

private:
  bool LoadTextureColor2D(const std::string& filename);
  bool LoadTextureCubeMap(const std::vector<std::string>& filenames);
};
