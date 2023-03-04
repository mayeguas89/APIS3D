#include "gl_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLTexture::GLTexture(): Texture() {}

GLTexture::~GLTexture() {}

bool GLTexture::Load(const std::string& filename)
{
  if (!std::filesystem::exists(filename))
    return false;

  int width, height, components;

  unsigned char* pixels = stbi_load(filename.c_str(), &width, &height, &components, 4);

  if (pixels == nullptr)
  {
    std::cout << "Could not load file " << filename << std::endl;
    return false;
  }
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  GLenum err = glGetError();

  size_ = glm::vec2(width, height);

  stbi_image_free(pixels);
  return true;
}

void GLTexture::Bind()
{
  if (id_ == 0)
    return;
  glBindTexture(GL_TEXTURE_2D, id_);
  glActiveTexture(GL_TEXTURE0);
}
