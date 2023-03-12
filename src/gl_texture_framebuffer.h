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

private:
  void CreateTexture();
  void AttachFrameBuffer();
  unsigned int fb_id_;
  FBType fb_type_;
}