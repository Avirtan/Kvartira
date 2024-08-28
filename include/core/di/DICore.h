#pragma once

#include "DIObject.h"
#include <memory>
#include <unordered_map>

namespace Core {

enum class DIObjects { ResourcesService, InputService };

class DiCore {
  public:
    DiCore() = delete;
    ~DiCore() = delete;

    static std::shared_ptr<DiObject> GetObject(DIObjects name) {
        auto pos = m_Objects.find(name);
        if (pos == m_Objects.end()) {
            return nullptr;
        }
        return pos->second;
    }

    static void SetObject(std::shared_ptr<DiObject> obj, DIObjects name) {
        if (GetObject(name) != nullptr) {
            return;
        }
        m_Objects.emplace(name, obj);
    }

  private:
    static inline std::unordered_map<DIObjects, std::shared_ptr<Core::DiObject>> m_Objects;
};
} // namespace Core