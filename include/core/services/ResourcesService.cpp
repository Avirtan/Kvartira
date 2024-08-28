#define GLM_ENABLE_EXPERIMENTAL
#include "ResourcesService.h"
#include "di/DICore.h"
#include "render/shader/Shader.h"
#include <SDL_log.h>
#include <fstream>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <sstream>
#include <string>

namespace CoreService {
ResourcesService::ResourcesService() {}
ResourcesService::~ResourcesService() {}

std::shared_ptr<Core::Shader> ResourcesService::GetShader(const std::string &vertName, const std::string &fragName) {
  // auto last = vertName.find_last_of("/");
  // if (last == std::string::npos) {
  //   return nullptr;
  // }
  // auto name = vertName.substr(last + 1, vertName.size());
  auto iter = m_Shaders.find(vertName);
  if (iter != m_Shaders.end()) {
    // std::cout << "get from cache count " << iter->second.use_count() << std::endl;
    return iter->second;
  }
  std::shared_ptr<Core::Shader> shader = std::make_shared<Core::Shader>();
  bool isValid = shader->Load(vertName, fragName);
  if (!isValid) {
    return nullptr;
  }
  m_Shaders.emplace(vertName, shader);
  // std::cout << "create shader" << std::endl;
  return shader;
}

std::shared_ptr<Core::Texture> ResourcesService::GetTexture(const std::string &path) {
  auto iter = m_Textures.find(path);
  if (iter != m_Textures.end()) {
    // std::cout << "get from cache count " << iter->second.use_count() << std::endl;
    return iter->second;
  }
  std::shared_ptr<Core::Texture> texture = std::make_shared<Core::Texture>();
  bool isValid = texture->Load(path);
  if (!isValid) {
    return nullptr;
  }
  m_Textures.emplace(path, texture);
  // std::cout << "create texture" << std::endl;
  return texture;
}

std::shared_ptr<Core::Mesh> ResourcesService::GetMesh(const std::string &path) {
  auto iter = m_Meshes.find(path);
  if (iter != m_Meshes.end()) {
    // std::cout << "get from cache count " << iter->second.use_count() << std::endl;
    return iter->second;
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    SDL_Log("File not found: Mesh %s", path.c_str());
    return nullptr;
  }

  std::stringstream fileStream;
  fileStream << file.rdbuf();
  std::string contents = fileStream.str();
  rapidjson::StringStream jsonStr(contents.c_str());
  rapidjson::Document doc;
  doc.ParseStream(jsonStr);

  if (!doc.IsObject()) {
    SDL_Log("Mesh %s is not valid json", path.c_str());
    return nullptr;
  }

  // int ver = doc["version"].GetInt();

  auto vertShaderPath = doc["shader_vert"].GetString();
  auto fragShaderPath = doc["shader_frag"].GetString();

  auto shader = GetShader(vertShaderPath, fragShaderPath);

  // std::cout << shader << std::endl;

  size_t vertSize = 8;

  // Load textures
  const rapidjson::Value &textures = doc["textures"];
  if (!textures.IsArray() || textures.Size() < 1) {
    SDL_Log("Mesh %s has no textures, there should be at least one", path.c_str());
    return nullptr;
  }

  //   mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());
  std::vector<std::shared_ptr<Core::Texture>> meshTextures;
  meshTextures.reserve(textures.Size());
  for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {

    std::string texName = textures[i].GetString();
    std::shared_ptr<Core::Texture> t = GetTexture(texName);
    if (t == nullptr) {
      return nullptr;
    }
    // if (t == nullptr) {
    //   // Try loading the texture
    //   t = renderer->GetTexture(texName);
    //   if (t == nullptr) {
    //     t = renderer->GetTexture("../Assets/Default.png");
    //   }
    // }
    meshTextures.emplace_back(t);
  }

  // Load in the vertices
  const rapidjson::Value &vertsJson = doc["vertices"];
  if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
    SDL_Log("Mesh %s has no vertices", path.c_str());
    return nullptr;
  }

  std::vector<Core::VertexAttributeData> vertices;
  vertices.reserve(vertsJson.Size());
  auto mRadius = 0.0f;
  for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
    // For now, just assume we have 8 elements
    const rapidjson::Value &vert = vertsJson[i];
    if (!vert.IsArray() || vert.Size() != 8) {
      SDL_Log("Unexpected vertex format for %s", path.c_str());
      return nullptr;
    }

    // Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
    // mRadius = Math::Max(mRadius, pos.LengthSq());

    // Add the floats
    // for (rapidjson::SizeType i = 0; i < vert.Size(); i++) {
    //   vertices.emplace_back(static_cast<float>(vert[i].GetDouble()));
    // }
    // vertices.push_back({glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0, 0)});

    vertices.push_back({glm::vec3(static_cast<float>(vert[0].GetFloat()), static_cast<float>(vert[1].GetFloat()), static_cast<float>(vert[2].GetFloat())),
                        glm::vec3(static_cast<float>(vert[3].GetFloat()), static_cast<float>(vert[4].GetFloat()), static_cast<float>(vert[5].GetFloat())),
                        glm::vec2(static_cast<float>(vert[6].GetFloat()), static_cast<float>(vert[7].GetFloat()))});
  }

  //   // We were computing length squared earlier
  //   mRadius = Math::Sqrt(mRadius);

  // Load in the indices
  const rapidjson::Value &indJson = doc["indices"];
  if (!indJson.IsArray() || indJson.Size() < 1) {
    SDL_Log("Mesh %s has no indices", path.c_str());
    return nullptr;
  }

  std::vector<unsigned int> indices;
  indices.reserve(indJson.Size() * 3);
  for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
    const rapidjson::Value &ind = indJson[i];
    if (!ind.IsArray() || ind.Size() != 3) {
      SDL_Log("Invalid indices for %s", path.c_str());
      return nullptr;
    }

    indices.emplace_back(ind[0].GetUint());
    indices.emplace_back(ind[1].GetUint());
    indices.emplace_back(ind[2].GetUint());
  }
  std::shared_ptr<Core::VertexArray> vao = std::make_shared<Core::VertexArray>();
  vao->SetVertexBuffer(&vertices[0], vertices.size());
  vao->SetElementBuffer(&indices[0], indices.size());

  std::shared_ptr<Core::Mesh> mesh = std::make_shared<Core::Mesh>(vao, meshTextures, shader);
  m_Meshes.emplace(path, mesh);

  return mesh;
}

} // namespace CoreService