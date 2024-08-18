#pragma once

#include <cstdint>
namespace Ecs {
enum class StatusEntity { live, dead };

// class Entity {
// public:
//   Entity(std::uint32_t id) : m_id(id) {}
//   ~Entity() {}

//   std::uint32_t GetId() const { return m_id; }

//   StatusEntity GetStatus() const { return m_Status; }
//   void SetStatus(StatusEntity status) { m_Status = status; }

// private:
//   std::uint32_t m_id;
//   StatusEntity m_Status;
// };
using Entity = std::uint32_t;

} // namespace Ecs