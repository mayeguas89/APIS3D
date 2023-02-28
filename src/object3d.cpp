#include "object3d.h"

#include "factory_engine.h"
#include "pugixml.hpp"
#include "utils.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

Object3D::Object3D(): Object() {}

void Object3D::LoadDataFromFile(const std::string& filename)
{
  std::unordered_map<std::string, Texture*> textures;

  if (filename.ends_with("msh"))
  {
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
  else
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
    }
    std::string directory = filename.substr(0, filename.find_last_of('/'));

    for (unsigned int i = 0; i < scene->mRootNode->mNumMeshes; i++)
    {
      aiMesh* mesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];
      for (unsigned int i = 0; i < mesh->mNumVertices; i++)
      {
        // Vertex vertex;
        // process vertex positions, normals and texture coordinates
        // vertices.push_back(vertex);
      }
      // process indices

      // process material
      if (mesh->mMaterialIndex >= 0)
      {}

      // return Mesh(vertices, indices, textures);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++)
    {
      for (unsigned int j = 0; j < scene->mRootNode->mChildren[i]->mNumMeshes; j++)
      {
        aiMesh* mesh = scene->mMeshes[scene->mRootNode->mChildren[i]->mMeshes[j]];
        auto my_mesh = new Mesh3D();
        for (unsigned int k = 0; k < mesh->mNumVertices; k++)
        {
          Vertex v;
          v.position.x = mesh->mVertices[k].x;
          v.position.y = mesh->mVertices[k].y;
          v.position.z = mesh->mVertices[k].z;
          v.position.w = 1.0f;

          if (mesh->HasNormals())
          {
            v.normal.x = mesh->mNormals[k].x;
            v.normal.y = mesh->mNormals[k].y;
            v.normal.z = mesh->mNormals[k].z;
            v.normal.w = 0.0f;
          }

          if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
          {
            v.texture_coordinates.x = mesh->mTextureCoords[0][k].x;
            v.texture_coordinates.y = mesh->mTextureCoords[0][k].y;
          }
          else
            v.texture_coordinates = glm::vec2(0.0f, 0.0f);

          my_mesh->AddVertex(v);
        }

        for (unsigned int k = 0; k < mesh->mNumFaces; k++)
        {
          aiFace face = mesh->mFaces[k];
          for (unsigned int l = 0; l < face.mNumIndices; l++)
            my_mesh->AddIndex(face.mIndices[l]);
        }
        // process material
        if (mesh->mMaterialIndex >= 0)
        {
          aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
          aiString str;
          material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

          // Creamos material
          auto my_material = FactoryEngine::GetNewMaterial();
          if (!my_material)
            throw std::runtime_error("Selected backend does not support materials");
          // Usamos los shaders de vertices y de los fragmentos
          auto vextex_shader_filename = "data/program.vertex";
          auto fragment_shader_filename = "data/program.fragment";

          std::unordered_map<std::string, RenderType> program_map;
          program_map[vextex_shader_filename] = RenderType::Vertex;
          program_map[fragment_shader_filename] = RenderType::Fragment;

          my_material->LoadPrograms(program_map);

          // Añadimos la textura al material
          auto texture_filename = directory + "/" + std::string(str.C_Str());
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
          my_material->SetTexture(texture);
          my_mesh->SetMaterial(my_material);
        }

        AddMesh(my_mesh);

        // return Mesh(vertices, indices, textures);
      }
    }
  }
}
