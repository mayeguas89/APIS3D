#include "gl_texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLTexture::GLTexture(): Texture() {}

GLTexture::~GLTexture() {}

bool GLTexture::Load(const std::vector<std::string>& filenames, Type type)
{
  for (const auto& filename: filenames)
  {
    if (!std::filesystem::exists(filename))
      return false;
  }

  type_ = type;

  if (type_ == Texture::Type::kColor2D)
  {
    return LoadTextureColor2D(filenames[0]);
  }
  return LoadTextureCubeMap(filenames);
}

bool GLTexture::LoadTextureColor2D(const std::string& filename)
{
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

void GLTexture::Bind(unsigned int index)
{
  if (id_ == 0)
    return;
  glActiveTexture(GL_TEXTURE0 + index);
  (type_ == Type::kCubeMap) ? glBindTexture(GL_TEXTURE_CUBE_MAP, id_) : glBindTexture(GL_TEXTURE_2D, id_);
}

bool GLTexture::LoadTextureCubeMap(const std::vector<std::string>& filenames)
{
  int width, height, components;

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);

  int texture_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
  int i = 0;
  // Orden es Positiva_x, negativa_x, positiva_y, negativa_y, etc.
  for (const auto& filename: filenames)
  {
    auto pixels = stbi_load(filename.c_str(), &width, &height, &components, 4);

    if (pixels == nullptr)
    {
      std::cout << "Could not load file " << filename << std::endl;
      return false;
    }

    glTexImage2D(texture_target + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);

    i += 1;
  }

  type_ = Type::kCubeMap;

  return true;
}