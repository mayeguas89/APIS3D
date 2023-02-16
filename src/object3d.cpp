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

  // for (pugi::xml_node buffer = buffers_node.child("buffer"); buffer; buffer = buffers_node.next_sibling("buffer"))
  // {
  for (pugi::xml_node buffer: buffers_node.children("buffer"))
  {
    // Creamos material
    auto material = FactoryEngine::GetNewMaterial();
    if (!material)
      throw std::runtime_error("Selected backend does not support materials");

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

    // Usamos los shaders de vertices y de los fragmentos
    auto vextex_shader_filename = buffer.child("material").child("vShader").text().as_string();
    auto fragment_shader_filename = buffer.child("material").child("fShader").text().as_string();

    std::unordered_map<std::string, RenderType> program_map;
    program_map[vextex_shader_filename] = RenderType::Vertex;
    program_map[fragment_shader_filename] = RenderType::Fragment;

    material->LoadPrograms(program_map);
    material->SetTexture(texture);

    auto pos_coords_list = utils::SplitString<float>(buffer.child("coords").text().as_string(), ',');
    auto texture_coords_list = utils::SplitString<float>(buffer.child("texCoords").text().as_string(), ',');
    auto coord = pos_coords_list.begin();
    auto text = texture_coords_list.begin();

    std::vector<Vertex> vertices;
    auto mesh = new Mesh3D();
    while (coord != pos_coords_list.end() && text != texture_coords_list.end())
    {
      Vertex v;
      v.position.x = *coord++;
      v.position.y = *coord++;
      v.position.z = *coord++;
      v.position.w = 1.0f;

      v.texture_coordinates.x = *text++;
      v.texture_coordinates.y = *text++;

      v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

      mesh->AddVertex(v);
    }

    for (auto index: utils::SplitString<unsigned int>(buffer.child("indices").text().as_string(), ','))
    {
      mesh->AddIndex(index);
    }
    mesh->SetMaterial(material);

    AddMesh(mesh);
  }
}
