#pragma once

#include "di/DIObject.h"
#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "render/texture/Texture.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace CoreService {
class ResourcesService : public Core::DiObject {
public:
  ResourcesService();
  ~ResourcesService();

  std::shared_ptr<Core::Shader> GetShader(const std::string &vertName, const std::string &fragName);
  std::shared_ptr<Core::Texture> GetTexture(const std::string &path);
  std::shared_ptr<Core::Mesh> GetMesh(const std::string &path);

private:
  std::unordered_map<std::string, std::shared_ptr<Core::Shader>> m_Shaders{};
  std::unordered_map<std::string, std::shared_ptr<Core::Texture>> m_Textures{};
  std::unordered_map<std::string, std::shared_ptr<Core::Mesh>> m_Meshes{};
};

} // namespace CoreService