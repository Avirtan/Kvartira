#pragma once

#include "./GltfObjects.h"
#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <sys/types.h>
#include <vector>

namespace Models {
class GLTF {
  public:
    GLTF();
    ~GLTF();
    void Load(std::string &path);

    void ParseBuffers(rapidjson::Document &&doc);

  private:
    std::vector<std::shared_ptr<GltfObject::Buffer>> m_buffers;
};
} // namespace Models