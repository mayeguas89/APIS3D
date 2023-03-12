#include "gl_texture_framebuffer.h"

GLTextureFrameBuffer::GLTextureFrameBuffer(FBType type, int width, int height): Texture(), fb_type_{type}
{
  size_ = glm::vec2(width, height);
  switch (fb_type_)
  {
    case GLTextureFrameBuffer::FBType::kDepthFb:
      type_ = Texture::Type::kDepthFb;
      break;
    case GLTextureFrameBuffer::FBType::kColorFb:
      type_ = Texture::Type::kColorFb;
      break;
    default:
      break;
  }
  CreateTexture();
  AttachFrameBuffer();

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    throw std::runtime_error("Error Setting framebuffer\n");
  }
}

void GLTextureFrameBuffer::Bind(unsigned int index)
{
  if (id_ == 0)
    return;
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void GLTextureFrameBuffer::CreateTexture()
{
  // Create the texture for the fb
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  switch (fb_type_)
  {
    case GLTextureFrameBuffer::FBType::kDepthFb:
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_DEPTH_COMPONENT24,
                   (int)size_.x,
                   (int)size_.y,
                   0,
                   GL_DEPTH_COMPONENT,
                   GL_UNSIGNED_BYTE,
                   nullptr);

      break;
    case GLTextureFrameBuffer::FBType::kColorFb:
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)size_.x, (int)size_.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    default:
      break;
  }
}

void GLTextureFrameBuffer::AttachFrameBuffer()
{
  // Attach the fb to the texture
  glGenFramebuffers(1, &fb_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, fb_id_);
  switch (fb_type_)
  {
    case GLTextureFrameBuffer::FBType::kDepthFb:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, id_, 0);
      break;
    case GLTextureFrameBuffer::FBType::kColorFb:
      unsigned int fb_depth_id;
      glGenRenderbuffers(1, &fb_depth_id);
      glBindRenderbuffer(GL_RENDERBUFFER, fb_depth_id);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (int)size_.x, (int)size_.y);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fb_depth_id);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id_, 0);
    default:
      break;
  }
}

unsigned int GLTextureFrameBuffer::GetFrameBufferId()
{
  return fb_id_;
}
