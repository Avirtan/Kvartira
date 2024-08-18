#pragma once

#include "render/shader/Shader.h"
#include "di/DIObject.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace CoreService {
class ResourcesService : public Core::DiObject {
public:
  ResourcesService();
  ~ResourcesService();

  std::shared_ptr<Core::Shader> GetShader(const std::string &vertName,
                                          const std::string &fragName);

private:
  std::unordered_map<std::string, std::shared_ptr<Core::Shader>> m_Shaders;
};

} // namespace Core