#pragma once

#include <memory>
namespace Core {
class Actor;

class Component {
public:
  Component(std::unique_ptr<Core::Actor>&& owner, int order = 100);
  ~Component();

  void Update(float deltaTime);

  int GetOrder() const { return m_Order; }

private:
  class std::unique_ptr<Core::Actor> m_Actor;

  int m_Order;
};
} // namespace Core