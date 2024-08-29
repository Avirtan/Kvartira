#pragma once

#include "ecs/Component.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/quaternion_float.hpp"
#include <glm/ext.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace Components {
struct Transform : public Ecs::Component {
  public:
    Transform() : m_Position(glm::vec3(0, 0, 0)), m_Rotation(glm::fquat(1, 0, 0, 0)), m_Scale(glm::vec3(1.0f)), m_Matrix(glm::mat4(1.0f)) {}
    ~Transform() {}

    glm::mat4 GetMatrix() { return positionMatrix() * rotateMatrix() * scaleMatrix(); }

    glm::mat4 GetMatrixCamera() { return rotateMatrix() * positionMatrix(); }

    void SetMatrix(glm::mat4 &matrix) { m_Matrix = matrix; }

    glm::vec3 Right() { return glm::column(rotateMatrix(), 0); }
    glm::vec3 Up() { return glm::column(rotateMatrix(), 1); }
    glm::vec3 Forward() { return glm::column(rotateMatrix(), 2); }

    glm::vec3 &Position() { return m_Position; }
    void SetPosition(glm::vec3 &position) { m_Position = position; }
    void SetPositionCamera(glm::vec3 &position) { m_Position = -position; }

    void SetRotation(glm::fquat &rotation) { m_Rotation = rotation; }

    void SetRotateAxis(float angle, glm::vec3 axis) {
        glm::fquat rot = glm::normalize(glm::angleAxis(angle, axis));
        m_Rotation = rot;
    }

    void RotateAxis(float angle, glm::vec3 &axis) {
        glm::fquat rot = glm::normalize(glm::angleAxis(angle, axis));
        m_Rotation = m_Rotation * rot;
    }

    void RotatePitchX(float angle) {
        glm::fquat rot = glm::normalize(glm::angleAxis(angle, Right()));
        m_Rotation = m_Rotation * rot;
    }

    void RotateYawY(float angle) {
        glm::fquat rot = glm::angleAxis(angle, Up());
        m_Rotation = m_Rotation * rot;
    }

    void RotateRollZ(float angle) {
        glm::fquat rot = glm::normalize(glm::angleAxis(angle, Forward()));
        m_Rotation = m_Rotation * rot;
    }

  private:
    glm::vec3 m_Position;
    glm::fquat m_Rotation;
    glm::vec3 m_Scale;
    glm::mat4 m_Matrix;

    glm::mat4 positionMatrix() { return glm::translate(glm::mat4(1.0f), m_Position); }
    glm::mat4 rotateMatrix() { return glm::toMat4(m_Rotation); }
    glm::mat4 scaleMatrix() { return glm::scale(m_Scale); }
};
} // namespace Components