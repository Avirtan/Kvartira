#pragma once

#include "ecs/Component.h"
#include "render/shader/Shader.h"
#include "render/vertexArray/VertexArray.h"
#include <glm/ext.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>

namespace Components {
struct CameraComponent : public Ecs::Component {
public:
  CameraComponent(glm::mat4 projection) : m_Projection(projection) {}
  ~CameraComponent() {}

  void SetPositionAndTarget(const glm::vec3 &position,
                            const glm::vec3 &target = glm::vec3(0.0f)) {
    m_Position = position;
    m_Target = target;
    glm::mat4 view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
    m_VPMatrix = m_Projection * view;
  }

  const glm::vec3 &GetPosition() const { return m_Position; }
  const glm::mat4 &GetVP() const { return m_VPMatrix; }

private:
  glm::vec3 m_Position;
  glm::vec3 m_Target;
  glm::mat4 m_Projection;
  glm::mat4 m_VPMatrix;
};
} // namespace Components