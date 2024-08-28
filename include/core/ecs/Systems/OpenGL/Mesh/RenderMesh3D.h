#pragma once
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "ecs/Components/CameraComponent.h"
#include "ecs/Components/OpenGL/Mesh/MeshComponent.h"
#include "ecs/Components/OpenGL/Render/RenderComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "render/RenderUtils.h"
#include <GL/glew.h>
#include <glm/detail/type_quat.hpp>
#include <glm/ext.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

namespace RenderSystems {
class RenderMesh3D : public Ecs::System {
  public:
    RenderMesh3D() {}
    ~RenderMesh3D() {}

    void Init() override {}

    float time = 0;

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
        auto poolMesh2d = m_World->GetPoolComponent<Components::MeshComponent3D, Components::Transform>();
        auto poolCamera = m_World->GetPoolComponent<Components::CameraComponent, Components::Transform>();
        time += deltaTime;
        glm::mat4 matrixVP;
        glm::vec3 positionCamera;
        for (auto ent : poolCamera) {
            auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
            auto transform = m_World->GetComponent<Components::Transform>(ent);
            // std::cout << glm::to_string() << std::endl;
            matrixVP = camera->m_Projection * transform->GetMatrixCamera(); // camera->GetVP();
            // positionCamera = camera->Position();
        }
        if (poolMesh2d.size() > 0) {
            for (auto _ : pool) {
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);

                for (auto ent : poolMesh2d) {
                    auto mesh = m_World->GetComponent<Components::MeshComponent3D>(ent);
                    auto transform = m_World->GetComponent<Components::Transform>(ent);

                    auto forward = transform->Forward();
                    auto right = transform->Right();
                    // std::cout << std::endl;
                    // std::cout << glm::to_string(forward) << std::endl;

                    // auto forwardCamera = transform->GetMatrix() * glm::vec4(matrixVP[0][2], matrixVP[1][2], matrixVP[2][2], 1);
                    auto dir = positionCamera;
                    // std::cout << std::endl;

                    float cosTheta = dot(forward, glm::normalize(dir));
                    float cosTheta1 = dot(right, glm::normalize(dir));

                    bool isRigth = cosTheta1 != 0 && cosTheta1 > 0;
                    float sign = 1;
                    if (isRigth) {
                        sign = 1;
                        // std::cout << "справа " << std::endl;
                    } else {
                        sign = -1;
                        // std::cout << "слева " << std::endl;
                    }

                    if (cosTheta < 0 && time > 2) {
                        // auto m3 = glm::rotate(transform->GetMatrix(), glm::acos(cosTheta), glm::vec3(0.0f, 1.0f, 0.0f));
                        // std::cout << glm::to_string(m3) << std::endl;
                        // std::cout << "rotate " << std::endl;
                        // transform->SetMatrix(m3);
                        // time = 0;
                    }
                    // std::cout << dir << std::endl;

                    if (time > 1.0f) {
                        // std::cout << cosTheta1 << " right" << std::endl;
                        // std::cout << cosTheta << " forward" << std::endl;
                        // std::cout << glm::to_string(forward) << std::endl;
                        // std::cout << glm::to_string(glm::normalize(forward)) << std::endl;
                        // std::cout << glm::to_string(glm::normalize(dir)) << std::endl;
                        // std::cout << glm::to_string(matrixVP) << std::endl;

                        // std::cout << glm::degrees(glm::acos(cosTheta)) << std::endl;

                        // std::cout << std::endl;

                        // auto q = glm::angleAxis(glm::acos(cosTheta), glm::vec3(0.0f, 1.0f, 0.0f));

                        // std::cout << glm::to_string(q) << std::endl;
                        // auto m4 = glm::toMat4(q);
                        // auto m5 = glm::mat4(1.0f) * m4 * glm::mat4(1.0f);
                        // std::cout << glm::to_string(m4) << std::endl;//glm::acos(cosTheta)
                        // auto m3 = glm::rotate(transform->GetMatrix(), sign * glm::acos(glm::radians(cosTheta)), glm::vec3(0.0f, 1.0f, 0.0f));
                        // auto m3 = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                        // std::cout << glm::to_string(m3) << std::endl;
                        // transform->SetMatrix(m3);
                        time = 0;
                    }
                    auto position = transform->Position();

                    position += transform->Forward() * deltaTime;
                    transform->SetPosition(position);
                    // std::cout << glm::to_string(transform->GetMatrix()) << std::endl;
                    mesh->Shader->SetActive();
                    mesh->Shader->SetMatrixUniform("uWorldTransform", matrixVP * transform->GetMatrix());
                    glCheckError();

                    mesh->VertexArray->SetActive();
                    if (mesh->Texture != nullptr) {
                        mesh->Texture->SetActive();
                    }
                    glCheckError();

                    glDrawElements(GL_TRIANGLES, mesh->VertexArray->GetCountIndex(), GL_UNSIGNED_INT, 0);

                    glCheckError();
                }
                glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE);
            }
        }
    }
};
} // namespace RenderSystems