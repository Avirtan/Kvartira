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

    void SetPositionAndTarget(const glm::vec3 &position, const glm::vec3 &target) {
        m_Position = position;
        m_Target = target;
        // auto position1 = glm::vec3(0.0f, -2.0f, 8.0f);

        glm::mat4 view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
        // auto position = glm::vec3(3, 0, 0);

        auto m_orientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //;glm::fquat(0.7, 0, 1, 0);
        auto rMat = glm::toMat4(glm::fquat(1, 0, 0, 0));
        // auto forword = glm::vec3(glm::row(rMat, 2));
        // std::cout << glm::to_string(forword) << std::endl;

        // glm::mat4 view = glm::lookAt(position, forword, glm::vec3(0, 1, 0));
        // std::cout << glm::to_string(view) << std::endl;

        auto view2 = rMat * glm::translate(glm::mat4(1.0f), -position);
        // std::cout << glm::to_string(view2) << std::endl;
        // m_VPMatrix = m_Projection * view2;
    }

    const glm::vec3 &GetPosition() const { return m_Position; }
    const glm::mat4 &GetVP() const { return m_VPMatrix; }
    glm::vec3 Position() { return glm::vec3(m_VPMatrix[3][0], m_VPMatrix[3][1], m_VPMatrix[3][2]); }
    glm::mat4 m_Projection;

  private:
    glm::vec3 m_Position;
    glm::vec3 m_Target;

    glm::mat4 m_VPMatrix;
};
} // namespace Components