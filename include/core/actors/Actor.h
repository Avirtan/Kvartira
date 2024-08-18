#pragma once

#include "render/mesh/Mesh.h"
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>

namespace Core {
class Component;

class Actor {
public:
  Actor(std::unique_ptr<Core::Mesh>&& mesh);
  ~Actor();

  void Update(float deltaTime);
  void AddComponent(std::unique_ptr<Core::Component>&& component);
  virtual void Draw();

private:
  std::unique_ptr<Core::Mesh> m_Mesh;
  glm::mat4 m_Transform;
  std::vector<std::unique_ptr<Core::Component>> m_Components;
};
} // namespace Core