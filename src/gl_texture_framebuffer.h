#pragma once

#include "texture.h"

class GLTextureFrameBuffer: public Texture
{
public:
  enum class FBType
  {
    kDepthFb,
    kColorFb,
  };
  GLTextureFrameBuffer(FBType type, int width, int height);
  unsigned int GetFrameBufferId();
  bool Load(const std::vector<std::string>& filenames, Type type = Type::kColor2D)
  {
    return true;
  }
  void Bind(unsigned int index = 0) override;

private:
  void CreateTexture();
  void AttachFrameBuffer();
  unsigned int fb_id_;
  FBType fb_type_;
};