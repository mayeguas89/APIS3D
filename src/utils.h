#pragma once

#include "factory_engine.h"
#include "material.h"
#include "mesh3d.h"
#include "pugixml.hpp"

#include <sstream>
#include <string>
#include <vector>

namespace utils
{

template<typename T>
std::vector<T> SplitString(const std::string& str, char delim)
{
  std::vector<T> elems;
  if (str != "")
  {
    std::stringstream ss(str);
    std::string item;
    while (std::getline(ss, item, delim))
    {
      T value;
      std::stringstream ss2(item);
      ss2 >> value;
      elems.push_back(value);
    }
  }
  return elems;
}

inline Material* ProcessMaterial(pugi::xml_node buffer,
                                 std::unordered_map<std::string, Texture*>& textures,
                                 const std::string& directory)
{
  glm::vec4 material_color = glm::vec4(1.f, 1.f, 1.f, 1.f);

  // Creamos material
  auto material = FactoryEngine::GetNewMaterial();
  if (!material)
    throw std::runtime_error("Selected backend does not support materials");

  // Material with color
  if (auto attrib = buffer.child("material").child("color"); attrib != nullptr)
  {
    auto color_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    auto the_color = color_list.begin();
    while (the_color != color_list.end())
    {
      material_color.r = *the_color++;
      material_color.g = *the_color++;
      material_color.b = *the_color++;
      material_color.a = *the_color++;
    }
  }

  material->SetColor(material_color);

  // Shininess
  if (auto attrib = buffer.child("material").child("shininess"); attrib != nullptr)
  {
    material->SetShininess(attrib.text().as_int());
  }

  if (auto attrib = buffer.child("material").child("light"); attrib != nullptr)
  {
    material->SetLightEnabled(attrib.text().as_bool());
  }

  if (auto attrib = buffer.child("material").child("culling"); attrib != nullptr)
  {
    material->SetCullingEnabled(attrib.text().as_bool());
  }

  if (auto attrib = buffer.child("material").child("depthWrite"); attrib != nullptr)
  {
    material->SetDepthTestEnabled(attrib.text().as_bool());
  }

  if (auto attrib = buffer.child("material").child("blendMode"); attrib != nullptr)
  {
    std::string blend_as_string = attrib.text().as_string();
    Material::BlendMode blend_mode = Material::BlendMode::Solid;
    if (blend_as_string == "alpha")
      blend_mode = Material::BlendMode::Alpha;
    else if (blend_as_string == "add")
      blend_mode = Material::BlendMode::Add;
    else if (blend_as_string == "mul")
      blend_mode = Material::BlendMode::Mul;
    material->SetBlendMode(blend_mode);
  }

  // Usamos los shaders de vertices y de los fragmentos
  auto vextex_shader_filename = buffer.child("material").child("vShader").text().as_string();
  auto fragment_shader_filename = buffer.child("material").child("fShader").text().as_string();

  std::unordered_map<std::string, RenderType> program_map;
  program_map[vextex_shader_filename] = RenderType::Vertex;
  program_map[fragment_shader_filename] = RenderType::Fragment;

  material->LoadPrograms(program_map);

  // Material with texture
  if (auto attrib = buffer.child("material").child("texture"); attrib != nullptr)
  {
    // Añadimos la textura al material
    auto texture_filename = buffer.child("material").child("texture").text().as_string();
    Texture* texture = nullptr;
    if (textures.find(texture_filename) == textures.end())
    {
      texture = FactoryEngine::GetNewTexture();
      textures[texture_filename] = texture;
      std::string string_texture_filename{texture_filename};
      auto texture_file =
        string_texture_filename.substr(string_texture_filename.find_last_of('/'), string_texture_filename.back());
      if (!texture->Load(directory + texture_file))
      {
        std::string error_msg = "Error reading the texture file " + directory + texture_file + "\n";
        throw std::runtime_error(error_msg);
      }
      texture->Bind();
    }
    texture = textures[texture_filename];
    material->SetTexture(texture);
  }
  return material;
}

inline Mesh3D* ProcessMesh(pugi::xml_node buffer, Material* material)
{
  auto mesh = new Mesh3D();

  if (auto attrib = buffer.child("coords"); attrib != nullptr)
  {
    auto pos_coords_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    auto coord = pos_coords_list.begin();

    while (coord != pos_coords_list.end())
    {
      Vertex v;
      v.position.x = *coord++;
      v.position.y = *coord++;
      v.position.z = *coord++;
      v.position.w = 1.0f;

      v.color = glm::vec4(material->GetColor(), 1.0f);

      mesh->AddVertex(v);
    }
  }

  if (auto attrib = buffer.child("normals"); attrib != nullptr)
  {
    auto norm_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    auto norm = norm_list.begin();
    for (auto it = mesh->GetVertList()->begin(); it != mesh->GetVertList()->end() && norm != norm_list.end(); it++)
    {
      it->normal.x = *norm++;
      it->normal.y = *norm++;
      it->normal.z = *norm++;
      it->normal.w = 0.0f;
    }
  }

  if (auto attrib = buffer.child("texCoords"); attrib != nullptr)
  {
    auto texture_coords_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    auto text = texture_coords_list.begin();
    for (auto it = mesh->GetVertList()->begin();
         it != mesh->GetVertList()->end() && text != texture_coords_list.end();
         it++)
    {
      it->texture_coordinates.x = *text++;
      it->texture_coordinates.y = *text++;
    }
  }

  for (auto index: utils::SplitString<unsigned int>(buffer.child("indices").text().as_string(), ','))
  {
    mesh->AddIndex(index);
  }

  mesh->SetMaterial(material);

  return mesh;
}

}