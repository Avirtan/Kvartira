#include "ResourcesService.h"
#include "di/DICore.h"
#include "render/shader/Shader.h"
#include <iostream>
#include <memory>
#include <string>

namespace CoreService {
ResourcesService::ResourcesService() {}
ResourcesService::~ResourcesService() {}

std::shared_ptr<Core::Shader>
ResourcesService::GetShader(const std::string &vertName,
                            const std::string &fragName) {
  auto last = vertName.find_last_of("/");
  if (last == std::string::npos) {
    return nullptr;
  }
  auto name = vertName.substr(last + 1, vertName.size());
  auto iter = m_Shaders.find(name);
  if (iter != m_Shaders.end()) {
    // std::cout << "get from cache count " << iter->second.use_count() << std::endl;
    return iter->second;
  }
  std::shared_ptr<Core::Shader> shader = std::make_shared<Core::Shader>();
  bool isValid = shader->Load(vertName, fragName);
  if (!isValid) {
    return nullptr;
  }
  m_Shaders.emplace(name, shader);
  // std::cout << "create shader" << std::endl;
  return shader;
}

} // namespace Core