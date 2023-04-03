#pragma once

#include "factory_engine.h"
#include "gl_texture_framebuffer.h"
#include "material.h"
#include "mesh3d.h"
#include "pugixml.hpp"
#include "stb_image.h"
#ifdef ASSIMP_LOAD_ENABLE
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#endif
#include "armature.h"

#include <glm/gtx/quaternion.hpp>

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
  auto vertex_shader_filename = material_node.child("vShader").text().as_string();
  auto fragment_shader_filename = material_node.child("fShader").text().as_string();

  std::unordered_map<std::string, RenderType> program_map;
  program_map[vertex_shader_filename] = RenderType::Vertex;
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
      }

      if (material->GetReflection())
        material->SetReflectionTexture(texture);
      else if (material->GetRefraction())
        material->SetRefractionTexture(texture);
      material->SetBaseTexture(texture);
    }
    else if (auto color_fb_attrib = child_node.attribute("colorFB");
             color_fb_attrib != nullptr && color_fb_attrib.as_bool())
    {
      auto render = System::GetRender();
      material->SetBaseTexture(
        new GLTextureFrameBuffer(GLTextureFrameBuffer::FBType::kColorFb, render->GetWidth(), render->GetHeight()));
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
  std::vector<float> bone_index_list;
  auto bone_index_it = bone_index_list.end();
  std::vector<float> bone_weight_list;
  auto bone_weight_it = bone_weight_list.end();
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

  if (auto attrib = buffer.child("boneIndices"); attrib != nullptr)
  {
    bone_index_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    bone_index_it = bone_index_list.begin();
  }

  if (auto attrib = buffer.child("boneWeights"); attrib != nullptr)
  {
    bone_weight_list = utils::SplitString<float>(attrib.text().as_string(), ',');
    bone_weight_it = bone_weight_list.begin();
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

    if (!bone_index_list.empty() && bone_index_it != bone_index_list.end())
    {
      v.bone_indices.x = *bone_index_it++;
      v.bone_indices.y = *bone_index_it++;
      v.bone_indices.z = *bone_index_it++;
      v.bone_indices.w = *bone_index_it++;
    }

    if (!bone_weight_list.empty() && bone_weight_it != bone_weight_list.end())
    {
      v.bone_weight.x = *bone_weight_it++;
      v.bone_weight.y = *bone_weight_it++;
      v.bone_weight.z = *bone_weight_it++;
      v.bone_weight.w = *bone_weight_it++;
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

inline std::unique_ptr<Armature> GetArmatureFromMsfFile(const std::string filename)
{
  auto armature = std::make_unique<Armature>();

  auto directory = filename.substr(0, filename.find_last_of('/'));
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());

  if (!result)
  {
    std::string error_msg = "Error reading the file " + filename + "\nError: " + result.description();
    throw std::runtime_error(error_msg);
  }

  if (auto last_frame_node = doc.child("mesh").child("lastFrame"); last_frame_node != nullptr)
    armature->SetLastFrame(last_frame_node.text().as_uint());

  auto bones_node = doc.child("mesh").child("bones");

  int bone_index = 0;

  for (pugi::xml_node bone_node: bones_node.children("bone"))
  {
    std::string bone_name;
    std::string bone_parent_name;
    std::shared_ptr<Bone> bone_parent = nullptr;

    if (auto name_node = bone_node.child("name"); name_node != nullptr)
      bone_name = name_node.text().as_string();

    if (auto name_node = bone_node.child("parent"); name_node != nullptr)
    {
      bone_parent_name = name_node.text().as_string();
      bone_parent = armature->GetBone(bone_parent_name);
    }

    auto bone = std::make_shared<Bone>(bone_name, bone_parent, bone_index);

    if (auto inv_pose_node = bone_node.child("invPose"); inv_pose_node != nullptr)
    {
      auto inv_pose_list = SplitString<float>(inv_pose_node.text().as_string(), ',');
      auto inv_pose = glm::make_mat4(inv_pose_list.data());
      bone->SetInvPoseMatrix(inv_pose);
    }

    if (auto positions_node = bone_node.child("positions"); positions_node != nullptr)
    {
      auto positions_list = SplitString<float>(positions_node.text().as_string(), ',');
      auto it = positions_list.begin();
      while (it != positions_list.end())
      {
        auto frame = static_cast<unsigned int>(*it++);
        glm::vec3 position{1.f};
        position.x = *it++;
        position.y = *it++;
        position.z = *it++;
        bone->AddPosition(frame, position);
      }
    }

    if (auto rotations_node = bone_node.child("rotations"); rotations_node != nullptr)
    {
      auto rotations_list = SplitString<float>(rotations_node.text().as_string(), ',');
      auto it = rotations_list.begin();
      while (it != rotations_list.end())
      {
        auto frame = static_cast<unsigned int>(*it++);
        glm::quat rotation;
        rotation.w = *it++;
        rotation.x = *it++;
        rotation.y = *it++;
        rotation.z = *it++;
        bone->AddRotation(frame, rotation);
      }
    }
    if (auto scales_node = bone_node.child("scales"); scales_node != nullptr)
    {
      auto scales_list = SplitString<float>(scales_node.text().as_string(), ',');
      auto it = scales_list.begin();
      while (it != scales_list.end())
      {
        auto frame = static_cast<unsigned int>(*it++);
        glm::vec3 scale;
        scale.x = *it++;
        scale.y = *it++;
        scale.z = *it++;
        bone->AddScale(frame, scale);
      }
    }

    armature->AddBone(bone);
    bone_index++;
  }

  return std::move(armature);
}

#ifdef ASSIMP_LOAD_ENABLE
inline void ProcessMesh(aiMesh* mesh, Mesh3D* my_mesh)
{
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex v;
    v.position.x = mesh->mVertices[i].x;
    v.position.y = mesh->mVertices[i].y;
    v.position.z = mesh->mVertices[i].z;
    v.position.w = 1.0f;

    if (mesh->HasNormals())
    {
      v.normal.x = mesh->mNormals[i].x;
      v.normal.y = mesh->mNormals[i].y;
      v.normal.z = mesh->mNormals[i].z;
      v.normal.w = 0.0f;
    }

    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      v.texture_coordinates.x = mesh->mTextureCoords[0][i].x;
      v.texture_coordinates.y = mesh->mTextureCoords[0][i].y;
    }
    else
    {
      v.texture_coordinates = glm::vec2(0.0f, 0.0f);
    }

    if (mesh->HasTangentsAndBitangents())
    {
      v.tangent.x = mesh->mTangents[i].x;
      v.tangent.y = mesh->mTangents[i].y;
      v.tangent.z = mesh->mTangents[i].z;
      v.tangent.w = 1.0f;
    }

    if (mesh->HasVertexColors(0))
    {
      v.color.r = mesh->mColors[0][i].r;
      v.color.g = mesh->mColors[0][i].g;
      v.color.b = mesh->mColors[0][i].b;
      v.color.a = mesh->mColors[0][i].a;
    }

    my_mesh->AddVertex(v);
  }

  for (unsigned int k = 0; k < mesh->mNumFaces; k++)
  {
    aiFace face = mesh->mFaces[k];
    for (unsigned int l = 0; l < face.mNumIndices; l++)
      my_mesh->AddIndex(face.mIndices[l]);
  }
}

inline Material* ProcessMaterial(aiMesh* mesh, const aiScene* scene, const std::string& directory)
{
  stbi_set_flip_vertically_on_load(true);
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
    // Si la textura está en nuestro mapa de texturas la añadimos
    Texture* texture = System::GetTexture(std::string(str.C_Str()));
    if (texture == nullptr)
    {
      texture = FactoryEngine::GetNewTexture();
      System::AddTexture(str.C_Str(), texture);
      texture->Load({{texture_filename}});
      texture->Bind();
    }
    my_material->SetBaseTexture(texture);

    material->GetTexture(aiTextureType_NORMALS, 0, &str);
    Texture* normal_texture = System::GetTexture(std::string(str.C_Str()));
    texture_filename = directory + "/" + std::string(str.C_Str());
    if (normal_texture == nullptr)
    {
      normal_texture = FactoryEngine::GetNewTexture();
      System::AddTexture(str.C_Str(), normal_texture);
      texture->Load({{texture_filename}});
      texture->Bind();
    }
    my_material->SetNormalTexture(normal_texture);

    return my_material;
  }
  return nullptr;
}

inline std::vector<Mesh3D*> GetMeshesFromAssimp(const std::string filename)
{
  std::vector<Mesh3D*> to_return;
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return {};
  }
  std::string directory = filename.substr(0, filename.find_last_of('/'));
  for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++)
  {
    for (unsigned int j = 0; j < scene->mRootNode->mChildren[i]->mNumMeshes; j++)
    {
      aiMesh* mesh = scene->mMeshes[scene->mRootNode->mChildren[i]->mMeshes[j]];
      auto my_mesh = new Mesh3D();
      ProcessMesh(mesh, my_mesh);
      auto material = ProcessMaterial(mesh, scene, directory);
      if (material != nullptr)
        my_mesh->SetMaterial(material);
      to_return.push_back(my_mesh);
    }
  }
  return to_return;
}
#endif
}
