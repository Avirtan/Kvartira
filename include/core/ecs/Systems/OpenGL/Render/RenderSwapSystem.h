#pragma once
#include "./render/RenderUtils.h"
#include "ecs/Components/OpenGL/Render/RenderComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>

// #include "imgui.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_sdl2.h"

namespace RenderSystems {
class RenderSwapSystem : public Ecs::System {
  public:
    RenderSwapSystem() : Ecs::System(255) {}
    ~RenderSwapSystem() {}

    void Init() override {}

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
        for (auto ent : pool) {
            auto render = m_World->GetComponent<Components::RenderComponent>(ent);
            SDL_GL_SwapWindow(render->Window);
            glCheckError();
        }
    }

  private:
};
} // namespace RenderSystems