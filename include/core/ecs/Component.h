#pragma once

#include "ecs/Entity.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
namespace Ecs {

struct Component {};

class ArrayComponent {
public:
  ArrayComponent() {}
  ~ArrayComponent() {}

  const std::vector<Ecs::Entity> &GetEntities() const { return m_Entities; }

  void AddComponent(Ecs::Entity ent, std::shared_ptr<Component> component) {
    if (HasComponent(ent)) {
      return;
    }
    m_Components[ent] = component;
    m_Entities.push_back(ent);
  }

  void RemoveComponent(Ecs::Entity ent) {
    if (!HasComponent(ent)) {
      return;
    }
    auto iter = std::find(m_Entities.begin(), m_Entities.end(), ent);
    m_Entities.erase(iter);
    m_Components.erase(ent);
  }

  bool HasComponent(Ecs::Entity ent) {
    auto it = m_Components.find(ent);
    return it != m_Components.end();
  }

  template <typename T> std::shared_ptr<T> GetComponent(Ecs::Entity &ent) {
    auto it = m_Components.find(ent);
    if (it == m_Components.end()) {
      return nullptr;
    }
    return std::static_pointer_cast<T>(it->second);
  }

private:
  std::vector<Ecs::Entity> m_Entities{};
  std::unordered_map<Ecs::Entity, std::shared_ptr<Component>> m_Components{};
};
} // namespace Ecs