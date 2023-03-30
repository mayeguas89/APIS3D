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

inline Material* ProcessMaterial(pugi::xml_node buffer, const std::string& directory)
{
  auto material_node = buffer.child("material");
  if (!material_node)
    return nullptr;

  glm::vec4 material_color = glm::vec4(1.f, 1.f, 1.f, 1.f);

  // Creamos material
  auto material = FactoryEngine::GetNewMaterial();
  if (!material)
    throw std::runtime_error("Selected backend does not support materials");

  // Material with color
  if (auto attrib = material_node.child("color"); attrib != nullptr)
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

  if (auto attrib = material_node.child("shininess"); attrib != nullptr)
  {
    material->SetShininess(attrib.text().as_int());
  }

  if (auto attrib = material_node.child("light"); attrib != nullptr)
  {
    material->SetLightEnabled(attrib.text().as_bool());
  }

  if (auto attrib = material_node.child("culling"); attrib != nullptr)
  {
    material->SetCullingEnabled(attrib.text().as_bool());
  }

  if (auto attrib = material_node.child("depthWrite"); attrib != nullptr)
  {
    material->SetDepthTestEnabled(attrib.text().as_bool());
  }

  if (auto attrib = material_node.child("refraction"); attrib != nullptr)
  {
    material->SetRefraction(attrib.text().as_bool());

    if (auto attrib = material_node.child("refractCoef"); attrib != nullptr)
      material->SetRefractionCoefficient(attrib.text().as_float());
    else
      std::cout << "Material found with refraction activated but without a refraction coeficient\n";
  }

  if (auto attrib = material_node.child("reflection"); attrib != nullptr)
  {
    material->SetReflection(attrib.text().as_bool());
  }

  if (auto attrib = material_node.child("blendMode"); attrib != nullptr)
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
  auto vextex_shader_filename = material_node.child("vShader").text().as_string();
  auto fragment_shader_filename = material_node.child("fShader").text().as_string();

  std::unordered_map<std::string, RenderType> program_map;
  program_map[vextex_shader_filename] = RenderType::Vertex;
  program_map[fragment_shader_filename] = RenderType::Fragment;

  material->LoadPrograms(program_map);

  // Material with texture
  if (auto child_node = material_node.child("texture"); child_node != nullptr)
  {
    if (auto cube_map_attrib = child_node.attribute("cubeMap");
        cube_map_attrib != nullptr && cube_map_attrib.as_bool())
    {
      std::vector<std::string> texture_filenames;
      auto filenames = utils::SplitString<std::string>(child_node.text().as_string(), ',');
      auto texture_file = filenames.at(0).substr(filenames.at(0).find_last_of('/'), filenames.at(0).back());
      Texture* texture = System::GetTexture(texture_file);
      if (texture == nullptr)
      {
        texture = FactoryEngine::GetNewTexture();
        System::AddTexture(texture_file, texture);

        for (auto& filename: filenames)
        {
          auto texture_file = filename.substr(filename.find_last_of('/'), filename.back());
          texture_filenames.push_back(directory + texture_file);
        }

        if (!texture->Load(texture_filenames, Texture::Type::kCubeMap))
          throw std::runtime_error("Error reading the cubeMap texture files\n");
        texture->Bind((unsigned int)Texture::Type::kCubeMap);
      }

      if (material->GetReflection())
        material->SetReflectionTexture(texture);
      else if (material->GetRefraction())
        material->SetRefractionTexture(texture);
      material->SetBaseTexture(texture);
    }
    else
    {
      // Añadimos la textura al material
      auto texture_filename = child_node.text().as_string();
      std::string string_texture_filename{texture_filename};
      auto texture_file =
        string_texture_filename.substr(string_texture_filename.find_last_of('/'), string_texture_filename.back());

      Texture* texture = System::GetTexture(texture_file);

      if (texture == nullptr)
      {
        texture = FactoryEngine::GetNewTexture();
        System::AddTexture(texture_file, texture);

        if (!texture->Load({{directory + texture_file}}))
        {
          std::string error_msg = "Error reading the texture file " + directory + texture_file + "\n";
          throw std::runtime_error(error_msg);
        }
        texture->Bind((unsigned int)Texture::Type::kColor2D);
      }

      material->SetBaseTexture(texture);
    }
  }

  if (auto child_node = material_node.child("normalTexture"); child_node != nullptr)
  {
    // Añadimos la textura al material
    auto texture_filename = child_node.text().as_string();
    std::string string_texture_filename{texture_filename};
    auto texture_file =
      string_texture_filename.substr(string_texture_filename.find_last_of('/'), string_texture_filename.back());

    Texture* texture = System::GetTexture(texture_file);

    if (texture == nullptr)
    {
      texture = FactoryEngine::GetNewTexture();
      System::AddTexture(texture_file, texture);

      if (!texture->Load({{directory + texture_file}}, Texture::Type::kNormal))
      {
        std::string error_msg = "Error reading the texture file " + directory + texture_file + "\n";
        throw std::runtime_error(error_msg);
      }
      texture->Bind((unsigned int)Texture::Type::kNormal);
    }

    material->SetNormalTexture(texture);
  }

  return material;
}

inline void ProcessMesh(pugi::xml_node buffer, Mesh3D* mesh)
{
  std::vector<float> coord_list;
  auto coord_it = coord_list.end();
  std::vector<float> norm_list;
  auto norm_it = norm_list.end();
  std::vector<float> tang_list;
  auto tang_it = tang_list.end();
  std::vector<float> texture_coords_list;
  auto texture_it = texture_coords_list.end();
  std::vector<unsigned int> index_list;
  auto index_it = index_list.end();

  if (auto attrib = buffer.child("coords"); attrib != nullptr)
  {
    coord_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    coord_it = coord_list.begin();
  }
  if (auto attrib = buffer.child("normals"); attrib != nullptr)
  {
    norm_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    norm_it = norm_list.begin();
  }

  if (auto attrib = buffer.child("tangents"); attrib != nullptr)
  {
    tang_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    tang_it = tang_list.begin();
  }

  if (auto attrib = buffer.child("texCoords"); attrib != nullptr)
  {
    texture_coords_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    texture_it = texture_coords_list.begin();
  }

  if (auto attrib = buffer.child("indices"); attrib != nullptr)
  {
    index_list = utils::SplitString<unsigned int>(attrib.text().as_string(), ',');
    index_it = index_list.begin();
  }

  for (; coord_it != coord_list.end();)
  {
    Vertex v;
    v.position.x = *coord_it++;
    v.position.y = *coord_it++;
    v.position.z = *coord_it++;
    v.position.w = 1.0f;

    v.color = glm::vec4(mesh->GetMaterial()->GetColor(), 1.0f);

    if (!norm_list.empty() && norm_it != norm_list.end())
    {
      v.normal.x = *norm_it++;
      v.normal.y = *norm_it++;
      v.normal.z = *norm_it++;
      v.normal.w = 0.0f;
    }

    if (!tang_list.empty() && tang_it != tang_list.end())
    {
      v.tangent.x = *tang_it++;
      v.tangent.y = *tang_it++;
      v.tangent.z = *tang_it++;
      v.tangent.w = 0.0f;
    }

    if (!texture_coords_list.empty() && texture_it != texture_coords_list.end())
    {
      v.texture_coordinates.x = *texture_it++;
      v.texture_coordinates.y = *texture_it++;
    }

    mesh->AddVertex(v);
  }

  for (; index_it != index_list.end();)
    mesh->AddIndex(*index_it++);
}

inline std::vector<Mesh3D*> GetMeshesFromMshFile(const std::string filename)
{
  std::vector<Mesh3D*> to_return;

  auto directory = filename.substr(0, filename.find_last_of('/'));
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  if (!result)
  {
    std::string error_msg = "Error reading the file " + filename + "\nError: " + result.description();
    throw std::runtime_error(error_msg);
  }

  auto buffers_node = doc.child("mesh").child("buffers");

  for (pugi::xml_node buffer: buffers_node.children("buffer"))
  {
    auto mesh = new Mesh3D();

    auto material = utils::ProcessMaterial(buffer, directory);

    mesh->SetMaterial(material);

    utils::ProcessMesh(buffer, mesh);

    System::AddMesh(filename, mesh);

    to_return.push_back(mesh);
  }

  return to_return;
}
}
