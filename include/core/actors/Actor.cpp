#include "Actor.h"
#include "../components/Component.h"
#include "render/mesh/Mesh.h"
#include <glm/fwd.hpp>

namespace Core {
Actor::Actor(std::unique_ptr<Core::Mesh> &&mesh)
    : m_Mesh(std::move(mesh)), m_Transform(glm::mat4(1.0f)) {}

Actor::~Actor() {}

void Actor::Update(float deltaTime) {}

void Actor::Draw() {
  if (m_Mesh == nullptr) {
    return;
  }
  m_Mesh->GetShader()->SetActive();
  m_Mesh->Draw();
}

void Actor::AddComponent(std::unique_ptr<Core::Component> &&component) {
  int myOrder = component->GetOrder();
  auto iter = m_Components.begin();
  for (; iter != m_Components.end(); ++iter) {
    if (myOrder < (*iter)->GetOrder()) {
      break;
    }
  }
  m_Components.insert(iter, std::move(component));
}

} // namespace Core