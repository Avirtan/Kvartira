#pragma once

#include "ecs/Component.h"
#include <glm/ext.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace Components {
struct CameraComponent : public Ecs::Component {
  public:
    CameraComponent(glm::mat4 projection) : m_UpVector(glm::vec3(0.0f, 1.0f, 0.0f)), m_Projection(projection) {}
    ~CameraComponent() {}

    void SetPosition(const glm::vec3 &position) { m_Position = position; }
    void SetDirection(const glm::vec3 &dir) { m_Direction = dir; }

    const glm::vec3 &GetPosition() const { return m_Position; }

    glm::mat4 GetMatrixVP() {
        m_VPMatrix = m_Projection * GetViewMatrix();
        return m_VPMatrix;
    }

    glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Direction, m_UpVector); }

    glm::vec3 &Position() { return m_Position; }

    glm::vec3 Right() { return glm::normalize(glm::cross(m_Direction, m_UpVector)); }
    glm::vec3 Up() { return m_UpVector; }
    glm::vec3 Forward() { return m_Direction; }

  private:
    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_UpVector;
    glm::mat4 m_Projection;

    glm::mat4 m_VPMatrix;
};
} // namespace Components