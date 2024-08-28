#if 0
#pragma once
#include "ecs/Components/Render/MeshComponent.h"
#include "ecs/Components/Render/RenderComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "render/RenderUtils.h"
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <iostream>

// #include "imgui.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_sdl2.h"

namespace Systems {
class RenderSystem : public Ecs::System {
  public:
    RenderSystem(SDL_Window *window) : Ecs::System(255), m_Window(window) {}
    ~RenderSystem() {}

    void Init() override {
        std::cout << "init render " << std::endl;
        InitRender();
        auto renderComponent = std::make_shared<Components::RenderComponent>(Components::RenderComponent(m_Window, m_Context));
        auto entity = m_World->CreateEntity();
        m_World->AddComponent<Components::RenderComponent>(entity, renderComponent);
    }

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::RenderComponent>();
        auto poolMesh2d = m_World->GetPoolComponent<Components::MeshComponent2D, Components::Transform>();
        for (auto ent : pool) {
            auto render = m_World->GetComponent<Components::RenderComponent>(ent);

            // ImGuiIO &io = ImGui::GetIO();
            // ImGui_ImplOpenGL3_NewFrame();
            // ImGui_ImplSDL2_NewFrame();
            // ImGui::NewFrame();

            // if (show_demo_window)
            //   ImGui::ShowDemoWindow(&show_demo_window);

            {
                // static float f = 0.0f;
                // static int counter = 0;
                // ImGui::SetNextWindowPos(
                //     ImVec2(0, 0)); // Set the position of the new window

                // ImGui::Begin("Utils");
                // ImGui::Text("This is some useful text."); // Display some text (you
                // can
                //                                           // use a format strings
                //                                           too)
                // ImGui::Checkbox("Demo Window",
                //                 &show_demo_window);
                //                                     // open/close state
                // // ImGui::Checkbox("Another Window", &show_another_window);

                // ImGui::SliderFloat(
                //     "float", &f, 0.0f,
                //     1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
                // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3
                // floats representing a color

                // if (ImGui::Button(
                //         "Button")) // Buttons return true when clicked (most widgets
                //                    // return true when edited/activated)
                //   counter++;
                // ImGui::SameLine();
                // ImGui::Text("counter = %d", counter);

                // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                //             1000.0f / io.Framerate, io.Framerate);
                // ImGui::End();
            }

            // ImGui::Render();
            // glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (poolMesh2d.size() > 0) {
                glEnable(GL_BLEND);
                glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

                for (auto ent1 : poolMesh2d) {
                    auto mesh = m_World->GetComponent<Components::MeshComponent2D>(ent1);
                    auto transform = m_World->GetComponent<Components::Transform>(ent1);
                    mesh->Shader->SetActive();
                    mesh->Shader->SetMatrixUniform("uWorldTransform", transform->GetMatrix());
                    glCheckError();

                    mesh->VertexArray->SetActive();
                    glCheckError();

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                    glCheckError();
                }
                glDisable(GL_BLEND);
            }

            // glViewport((int)io.DisplaySize.x / 2, 0, (int)io.DisplaySize.x / 2,
            //            (int)io.DisplaySize.y);
            // glEnable(GL_SCISSOR_TEST);
            // glScissor((int)io.DisplaySize.x / 2, 0, (int)io.DisplaySize.x / 2,
            //           (int)io.DisplaySize.y);
            // glClearColor(1, 0.5, 0, 1);
            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // glDisable(GL_SCISSOR_TEST);
            // glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

            // if (poolMesh2d.size() > 0) {
            //   glEnable(GL_BLEND);
            //   glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            //   glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
            //                       GL_ZERO);

            //   for (auto ent1 : poolMesh2d) {
            //     auto mesh =
            //     m_World->GetComponent<Components::MeshComponent2D>(ent1); auto
            //     transform = m_World->GetComponent<Components::Transform>(ent1);
            //     mesh->Shader->SetActive();
            //     mesh->Shader->SetMatrixUniform("uWorldTransform",
            //     transform->Matrix); glCheckError();

            //     mesh->VertexArray->SetActive();
            //     glCheckError();

            //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            //     glCheckError();
            //   }
            //   glDisable(GL_BLEND);
            // }

            // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(m_Window);
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
} // namespace Systems
#endif