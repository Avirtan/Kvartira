#pragma once
#include "ecs/Components/CameraComponent.h"
#include "ecs/Components/OpenGL/Render/RenderComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "render/RenderUtils.h"
#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
// #include "imgui.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_sdl2.h"

namespace RenderSystems {
class RenderInitSystem : public Ecs::System {
  public:
    RenderInitSystem(SDL_Window *window) : Ecs::System(0), m_Window(window) {}
    ~RenderInitSystem() {}

    void Init() override {

        InitRender();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1024 / 768, 0.1f, 100.0f);

        auto cameraComponent = std::make_shared<Components::CameraComponent>(Components::CameraComponent(projection));

        auto position = glm::vec3(0.0f, 0.0f, 8.0f);
        auto dir = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraComponent->SetPosition(position);
        cameraComponent->SetDirection(dir);
        auto entityCamera = m_World->CreateEntity();
        m_World->AddComponent<Components::CameraComponent>(entityCamera, cameraComponent);

        auto renderComponent = std::make_shared<Components::RenderComponent>(Components::RenderComponent(m_Window, m_Context));
        auto entity = m_World->CreateEntity();
        m_World->AddComponent<Components::RenderComponent>(entity, renderComponent);
    }

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
        for (auto ent : pool) {
            auto render = m_World->GetComponent<Components::RenderComponent>(ent);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void InitRender() {
        SDL_GLprofile profile = SDL_GL_CONTEXT_PROFILE_CORE;
        std::pair<short, short> version = std::pair(4, 3);
        short sizeColor = 8;
        short sizeDepth = 24;
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.first);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.second);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, sizeColor);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, sizeColor);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, sizeColor);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, sizeColor);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sizeDepth);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        m_Context = SDL_GL_CreateContext(m_Window);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            SDL_Log("Failed to initialize GLEW.");
        }
        glCheckError();
    }

  private:
    SDL_Window *m_Window;
    SDL_GLContext m_Context;
};
} // namespace RenderSystems