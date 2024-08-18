#pragma once

#include "./System.h"
#include "ecs/Component.h"
#include "ecs/Entity.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace Ecs {
class System;

class World : public std::enable_shared_from_this<World> {

public:
  World(std::uint32_t countEntity = 1000) : m_CountUpdatePool(0) {
    m_CountEntity = countEntity > 2 ? countEntity : 2;
    for (std::size_t i = 0; i < countEntity; i++) {
      m_EntitiesPool.push(i);
    }
    m_EntitiesLive.reserve(countEntity);
  }
  ~World() {}

  Ecs::Entity CreateEntity() {
    if (m_EntitiesPool.empty()) {
      m_CountUpdatePool++;
      m_EntitiesLive.reserve(m_CountEntity * m_CountUpdatePool);
      for (std::size_t i = 0; i < m_CountEntity / 2; i++) {
        m_EntitiesPool.push(i);
      }
    }
    auto entity = m_EntitiesPool.front();
    m_EntitiesPool.pop();
    m_EntitiesLive.push_back(entity);
    return entity;
  }

  void DropEntity(const Ecs::Entity &entity) {
    if (!IsAliveEntity(entity)) {
      std::cout << "повторное удаление" << std::endl;
      return;
      ;
    }
    auto iter = std::find(m_EntitiesLive.begin(), m_EntitiesLive.end(), entity);
    if (iter != m_EntitiesLive.end()) {
      std::cout << "Drop entity " << std::endl;
      for (auto component : m_Components) {
        component.second->RemoveComponent(entity);
      }
      auto entity = *iter;
      m_EntitiesLive.erase(iter);
      m_EntitiesPool.push(entity);
    }
  }

  bool IsAliveEntity(const Ecs::Entity &entity) {
    auto iter = std::find(m_EntitiesLive.begin(), m_EntitiesLive.end(), entity);
    return iter != m_EntitiesLive.end();
  }

  template <typename T>
  void AddComponent(Ecs::Entity &entity, std::shared_ptr<T> &component) {
    auto name = typeid(T).name();
    auto it = m_Components.find(name);
    if (it == m_Components.end()) {
      m_Components[name] = std::make_shared<Ecs::ArrayComponent>();
    }
    // std::cout << name << " " << std::endl;
    auto arrayComponents = m_Components[name];
    arrayComponents->AddComponent(entity, component);
  }

  template <typename T> std::shared_ptr<T> GetComponent(Ecs::Entity &entity) {
    if (!IsAliveEntity(entity)) {
      return nullptr;
    }
    auto name = typeid(T).name();
    // std::cout << name << " " << std::endl;
    auto it = m_Components.find(name);
    if (it == m_Components.end()) {
      std::cout << "not found com" << std::endl;
      return nullptr;
    }

    auto component =
        std::static_pointer_cast<T>(it->second->GetComponent<T>(entity));
    return component;
  }

  void UpdateSystem(float deltaTime) {
    for (auto system : m_Systems) {
      system->Update(deltaTime);
    }
  }

  void InitSystem() {
    for (auto system : m_Systems) {
      system->Init();
    }
  }

  template <typename T> void AddSystem(std::shared_ptr<T> &system) {
    auto order = system->GetOrder();
    auto iter = m_Systems.begin();
    for (; iter != m_Systems.end(); ++iter) {
      if (order < (*iter)->GetOrder()) {
        break;
      }
    }
    system->SetWorld(shared_from_this());
    m_Systems.insert(iter, system);
  }

  template <typename T> std::vector<Ecs::Entity> GetPoolComponent() {
    auto name = typeid(T).name();
    std::vector<Ecs::Entity> vector{};
    auto it = m_Components.find(name);
    if (it != m_Components.end()) {
      vector = it->second->GetEntities();
    }
    return vector;
  }

  template <typename T, typename K>
  std::vector<Ecs::Entity> GetPoolComponent() {
    auto name1 = typeid(T).name();
    auto name2 = typeid(K).name();
    std::vector<Ecs::Entity> vector1{};
    std::vector<Ecs::Entity> vector2{};

    auto it = m_Components.find(name1);
    if (it != m_Components.end()) {
      vector1 = it->second->GetEntities();
    }

    it = m_Components.find(name2);
    if (it != m_Components.end()) {
      vector2 = it->second->GetEntities();
    }

    std::vector<Ecs::Entity> vectorResult{};
    auto size1 = vector1.size();
    auto size2 = vector2.size();

    vectorResult.reserve(size1 + size2);

    auto min = std::min(size1, size2);
    if (min == size1) {
      for (auto e : vector1) {
        if (std::find(vector2.begin(), vector2.end(), e) != vector2.end()) {
          vectorResult.push_back(e);
        }
      }
    } else {
      for (auto e : vector2) {
        if (std::find(vector1.begin(), vector1.end(), e) != vector1.end()) {
          vectorResult.push_back(e);
        }
      }
    }

    return vectorResult;
  }

private:
  std::queue<Ecs::Entity> m_EntitiesPool{};
  std::vector<Ecs::Entity> m_EntitiesLive{};
  std::vector<std::shared_ptr<Ecs::System>> m_Systems;

  std::unordered_map<std::string, std::shared_ptr<Ecs::ArrayComponent>>
      m_Components;

  std::uint32_t m_CountEntity;
  std::uint8_t m_CountUpdatePool;
};
} // namespace Ecs