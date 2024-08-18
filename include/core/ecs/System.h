#pragma once

#include <cstdint>
#include <memory>
namespace Ecs {
class World;

class System {
public:
  System(std::uint8_t order = 100) : m_Order(order) {}
  virtual ~System() = default;

  virtual void Init() {}
  virtual void Update(float deltaTime) {}
  virtual void Destroy() {}

  void SetWorld(const std::shared_ptr<Ecs::World> &world) { m_World = world; }
  std::uint8_t GetOrder() const { return m_Order; }

protected:
  std::shared_ptr<Ecs::World> m_World;

private:
  std::uint8_t m_Order;
};
} // namespace Ecs