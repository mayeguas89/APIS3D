#include "object3d.h"

#include "factory_engine.h"
#include "pugixml.hpp"
#include "utils.h"

Object3D::Object3D(): Object() {}

void Object3D::LoadDataFromFile(const std::string& filename)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  if (!result)
  {
    std::string error_msg = "Error reading the file " + filename + "\nError: " + result.description();
    throw std::runtime_error(error_msg);
  }

  auto buffers_node = doc.child("mesh").child("buffers");

  std::unordered_map<std::string, Texture*> textures;

  for (pugi::xml_node buffer: buffers_node.children("buffer"))
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
      // Si la textura está en nuestro mapa de texturas la añadimos
      if (textures.find(texture_filename) == textures.end())
      {
        texture = FactoryEngine::GetNewTexture();
        textures[texture_filename] = texture;
        texture->Load(texture_filename);
        texture->Bind();
      }
      texture = textures[texture_filename];
      material->SetTexture(texture);
    }

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

        v.color = material_color;

        mesh->AddVertex(v);
      }
    }

    if (auto attrib = buffer.child("normals"); attrib != nullptr)
    {
      auto norm_list = utils::SplitString<float>(attrib.text().as_string(), ',');
      auto norm = norm_list.begin();
      for (auto it = mesh->GetVertList()->begin(); it != mesh->GetVertList()->end() && norm != norm_list.end();
           it++)
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

    AddMesh(mesh);
  }
}
