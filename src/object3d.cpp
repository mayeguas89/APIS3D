#include "object3d.h"

#include "factory_engine.h"
#include "pugixml.hpp"
#include "system.h"
#include "utils.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

Object3D::Object3D(): Object() {}

void Object3D::LoadDataFromFile(const std::string& filename)
{
  std::unordered_map<std::string, Texture*> textures;

  if (const auto meshes = System::GetMesh(filename); !meshes.empty())
  {
    for (auto mesh: meshes)
      AddMesh(mesh);
    return;
  }

  if (filename.ends_with("msh"))
  {
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
      auto material = utils::ProcessMaterial(buffer, textures, directory);
      auto mesh = utils::ProcessMesh(buffer, material);

      System::AddMesh(filename, mesh);

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
