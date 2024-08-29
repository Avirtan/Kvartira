#pragma once
#include "ecs/Components/CameraComponent.h"
#include "ecs/Components/OpenGL/Mesh/MeshComponent.h"
#include "ecs/Components/OpenGL/Render/RenderComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "render/RenderUtils.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

namespace RenderSystems {
class RenderMesh2D : public Ecs::System {
  public:
    RenderMesh2D() {}
    ~RenderMesh2D() {}

    void Init() override {}

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
        auto poolMesh2d = m_World->GetPoolComponent<Components::MeshComponent2D, Components::Transform>();
        auto poolCamera = m_World->GetPoolComponent<Components::CameraComponent>();

        glm::mat4 matrixVP;
        for (auto ent : poolCamera) {
            auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
            matrixVP = camera->GetMatrixVP();
        }
        if (poolMesh2d.size() > 0) {
            for (auto _ : pool) {
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

                for (auto ent : poolMesh2d) {
                    auto mesh = m_World->GetComponent<Components::MeshComponent2D>(ent);
                    auto transform = m_World->GetComponent<Components::Transform>(ent);
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
                glDisable(GL_BLEND);
            }
        }
    }

  private:
};
} // namespace RenderSystems