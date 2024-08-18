#pragma once
#include "ecs/Components/CameraComponent.h"
#include "ecs/Components/MeshComponent.h"
#include "ecs/Components/RenderComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "render/RenderUtils.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

namespace Systems {
class RenderMesh2D : public Ecs::System {
public:
  RenderMesh2D() {}
  ~RenderMesh2D() {}

  void Init() override {}

  void Update(float deltaTime) override {
    auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
    auto poolMesh2d = m_World->GetPoolComponent<Components::MeshComponent2D,
                                                Components::Transform>();
    auto poolCamera = m_World->GetPoolComponent<Components::CameraComponent>();

    glm::mat4 matrixVP;
    for (auto ent : poolCamera) {
      auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
      matrixVP = camera->GetVP();
    }
    if (poolMesh2d.size() > 0) {
      for (auto _ : pool) {
        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
                            GL_ZERO);

        for (auto ent : poolMesh2d) {
          auto mesh = m_World->GetComponent<Components::MeshComponent2D>(ent);
          auto transform = m_World->GetComponent<Components::Transform>(ent);
          mesh->Shader->SetActive();
          mesh->Shader->SetMatrixUniform("uWorldTransform", matrixVP * transform->Matrix);
          glCheckError();

          mesh->VertexArray->SetActive();
          glCheckError();

          glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

          glCheckError();
        }
        glDisable(GL_BLEND);
      }
    }
  }

private:
};
} // namespace Systems