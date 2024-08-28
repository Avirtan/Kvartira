#include "./GameManager.h"

#include <GL/glew.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <cstddef>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "../systems/RotateCameraSystem.h"
#include "di/DICore.h"
#include "ecs/Components/OpenGL/Mesh/MeshComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/Systems/MoveSystem.h"
#include "ecs/Systems/OpenGL/Mesh/RenderMesh2D.h"
#include "ecs/Systems/OpenGL/Mesh/RenderMesh3D.h"
#include "ecs/Systems/OpenGL/Render/RenderInitSystem.h"
#include "ecs/Systems/OpenGL/Render/RenderSwapSystem.h"
#include "ecs/Systems/OpenGL/Render/RenderSystem.h"
#include "ecs/World.h"
#include "render/vertexArray/VertexArray.h"
#include "services/InputService.h"
#include "services/ResourcesService.h"

// #include "imgui.h"
// #include "imgui_impl_opengl3.h"
// #include "imgui_impl_sdl2.h"
// #include <filesystem>

namespace Managers {
GameManager::GameManager() : mIsRunning(false), mTicksCount(0), m_Window(nullptr) {}

bool GameManager::Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    int screenWidth = 1024;
    int screenHeight = 768;
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("kvrtira", 0, 0, screenWidth, screenHeight, window_flags);
    if (!m_Window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    CreateService();
    CreateSystem();

    m_World->InitSystem();

    LoadData();
    // SDL_SetWindowGrab(m_Window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    mIsRunning = true;
    return true;
}

void GameManager::CreateSystem() {
    m_World = std::make_shared<Ecs::World>(Ecs::World());

    auto renderInitSystem = std::make_shared<RenderSystems::RenderInitSystem>(RenderSystems::RenderInitSystem(m_Window));
    m_World->AddSystem<RenderSystems::RenderInitSystem>(renderInitSystem);

    auto renderSwapSystem = std::make_shared<RenderSystems::RenderSwapSystem>(RenderSystems::RenderSwapSystem());
    m_World->AddSystem<RenderSystems::RenderSwapSystem>(renderSwapSystem);

    auto renderMesh2DSystem = std::make_shared<RenderSystems::RenderMesh2D>(RenderSystems::RenderMesh2D());
    m_World->AddSystem<RenderSystems::RenderMesh2D>(renderMesh2DSystem);

    auto renderMesh3DSystem = std::make_shared<RenderSystems::RenderMesh3D>(RenderSystems::RenderMesh3D());
    m_World->AddSystem<RenderSystems::RenderMesh3D>(renderMesh3DSystem);

    auto moveSystem = std::make_shared<Systems::RotateCameraSystem>();
    m_World->AddSystem<Systems::RotateCameraSystem>(moveSystem);
}

void GameManager::CreateService() {
    auto resourceService = std::make_shared<CoreService::ResourcesService>();
    Core::DiCore::SetObject(resourceService, Core::DIObjects::ResourcesService);

    m_InputeService = std::make_shared<CoreService::InputService>();
    Core::DiCore::SetObject(m_InputeService, Core::DIObjects::InputService);
    m_InputeService->SetBinding(SDL_Scancode::SDL_SCANCODE_W, CoreService::InputAction::UP);
    m_InputeService->SetBinding(SDL_Scancode::SDL_SCANCODE_S, CoreService::InputAction::DOWN);
    m_InputeService->SetBinding(SDL_Scancode::SDL_SCANCODE_A, CoreService::InputAction::LEFT);
    m_InputeService->SetBinding(SDL_Scancode::SDL_SCANCODE_D, CoreService::InputAction::RIGHT);

    m_InputeService->SetBinding(SDL_Scancode::SDL_SCANCODE_ESCAPE, CoreService::InputAction::QUIT);
}

void GameManager::Run() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
    }
}

void GameManager::ProcessInput() {
    SDL_Event event;
    //
    while (SDL_PollEvent(&event)) {
        // ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                // std::cout << event.window.data1 << std::endl;
            }
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            // std::cout << (KMOD_ALT) << std::endl;
            // std::cout << (event.key.keysym.scancode) << std::endl;
            m_InputeService->SetKeyEvent(event.key);
            break;
        case SDL_MOUSEMOTION:
            m_InputeService->SetMouseMotionPosition(event.motion.xrel, event.motion.yrel);
            // m_InputeService->SetMousePosition(event.motion.x, event.motion.y);
            break;
        }
        // m_InputeService->SetMouseMotion(0, 0);
    }
    // SDL_Scancode
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_A]) {
        // SDL_GetScancodeFromName
        // SDL_Scancode::SDL_SCANCODE_KP_000;
        // std::cout << state[SDL_SCANCODE_A] << std::endl;
    }
    // std::cout << state[SDL_SCANCODE_A]  << std::endl;
    // for (auto k : state) {
    //   std::cout << k << std::endl;
    // }
    if (m_InputeService->GetValueAction(CoreService::InputAction::QUIT)) {
        mIsRunning = false;
    }
    // std::cout << m_InputeService->GetValueAction(CoreService::InputAction::QUIT) << std::endl;
}

void GameManager::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks64(), mTicksCount + 33))
        ;

    float deltaTime = (SDL_GetTicks64() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks64();

    m_World->UpdateSystem(deltaTime);
}

void GameManager::LoadData() {
    // std::cout << "Current path is " << std::filesystem::current_path() <<
    // '\n';

    std::string path = "./shaders/Sprite.vert";
    std::string path1 = "./shaders/Sprite.frag";
    std::string pathT = "./assets/Plane.png";

    std::string pathMesh = "./assets/mesh/3dCube.json";
    // std::string pathMesh = "./assets/mesh/sprite.json";

    auto r = std::static_pointer_cast<CoreService::ResourcesService>(Core::DiCore::GetObject(Core::DIObjects::ResourcesService));

    // auto shader = r->GetShader(path, path1);
    // auto texture = r->GetTexture(pathT);

    // std::shared_ptr<Core::VertexArray> vao = mesh->GetVertexArray(); //
    // std::make_shared<Core::VertexArray>(); Core::VertexAttributeData data[] =
    // {
    //     {glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0, 0)},
    //     {glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 0)},
    //     {glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 1)},
    //     {glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0,
    //     1)},
    // };
    // // for (auto d : data) {
    // //   std::cout << glm::to_string(d.Position) << std::endl;
    // // }
    // unsigned int indices[] = {0, 1, 2, 2, 3, 0};
    // vao->SetVertexBuffer(data, 4);
    // vao->SetElementBuffer(indices, 6);

    auto mesh = r->GetMesh(pathMesh);
    auto entity = m_World->CreateEntity();
    auto component = std::make_shared<Components::MeshComponent3D>(mesh);
    m_World->AddComponent<Components::MeshComponent3D>(entity, component);
    auto transform = std::make_shared<Components::Transform>();
    // auto m1 = glm::translate(transform->GetMatrix(), glm::vec3(1.0f, -1.0f, 0.0f));
    // m1 = glm::rotate(m1, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // transform->SetMatrix(m1);
    transform->RotateYawY(glm::radians(15.0f));
    // std::cout << glm::to_string(transform->GetMatrix()) << std::endl;

    // std::cout << glm::to_string(transform->Right()) << std::endl;
    auto position = glm::vec3(0.0f, 0.0f, 0.0f);
    // position += transform->Forward() * 4;
    transform->SetPosition(position);
    // std::cout << glm::to_string(position) << std::endl;

    m_World->AddComponent<Components::Transform>(entity, transform);
    // std::cout << glm::to_string(transform->GetMatrix()) << std::endl;
    // std::cout << glm::to_string(transform->GetMatrix()[0]) << std::endl;
    // std::cout << glm::to_string(glm::row(transform->GetMatrix(), 2)) << std::endl;
    // std::cout << glm::to_string(glm::vec3(transform->GetMatrix()[0][2], transform->GetMatrix()[1][2], transform->GetMatrix()[2][2])) << std::endl;
    // std::cout << glm::to_string(glm::column(transform->GetMatrix(), 3)) << std::endl;

    // auto position = glm::vec3(3, 0, 0);
    // auto m_orientation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //;glm::fquat(0.7, 0, 1, 0);
    // auto rMat = glm::toMat4(m_orientation);
    // auto forword = glm::vec3(glm::row(rMat, 2));
    // std::cout << glm::to_string(forword) << std::endl;

    // glm::mat4 view = glm::lookAt(position, forword, glm::vec3(0, 1, 0));
    // std::cout << glm::to_string(view) << std::endl;

    // auto view2 = rMat * glm::translate(glm::mat4(1.0f), -position);
    // std::cout << glm::to_string(view2) << std::endl;

    // auto entity1 = m_World->CreateEntity();
    // auto component1 = std::make_shared<Components::MeshComponent2D>(mesh);
    // m_World->AddComponent<Components::MeshComponent2D>(entity1, component1);
    // auto transform1 = std::make_shared<Components::Transform>();
    // transform1->Matrix = glm::rotate(transform1->Matrix, glm::radians(50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // transform1->Matrix = glm::translate(transform->Matrix, glm::vec3(4.0, 1.0, 0.0));
    // m_World->AddComponent<Components::Transform>(entity1, transform1);
    // transform->Matrix =
    //     glm::translate(transform->Matrix, glm::vec3(0.0, -1.0, 0.0));
    // auto tmpMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.0, 0.0));
    // auto v = glm::vec3(0.0f, 0.0f, 1.0f);
    // auto MyQuaternion = glm::angleAxis(glm::radians(45.0f), v);
    // auto tmpMat = glm::toMat4(MyQuaternion);

    // glm::transpose(tmpMat);

    // transform->Matrix *= tmpMat2;
    // transform->Matrix *= tmpMat;

    // auto tmpMat2 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.0, 0.0));
    // transform->Matrix = glm::mat4(1.0f);

    // glm::vec3 euler = glm::eulerAngles(MyQuaternion);

    // std::cout << glm::to_string(euler * 180 / 3.14159f) << std::endl;
    // std::cout << entity << std::endl;
    // std::unique_ptr<Core::VertexArray> vao2 =
    //     std::make_unique<Core::VertexArray>();
    // vao2->SetVertexBuffer(data, 4);
    // vao2->SetElementBuffer(indices, 6);

    // auto entity2 = m_World->CreateEntity();
    // auto component2 =
    //     std::make_shared<Components::MeshComponent2D>(std::move(vao2),
    //     shader);
    // m_World->AddComponent<Components::MeshComponent2D>(entity2, component2);
    // auto transform2 = std::make_shared<Components::Transform>();
    // transform2->Matrix =
    //     glm::translate(transform2->Matrix, glm::vec3(0.5, 0.5, 0.0));

    // transform2->Matrix = glm::rotate(transform2->Matrix, glm::radians(45.0f),
    //                                  glm::vec3(0.0f, 0.0f, 1.0f));

    // m_World->AddComponent<Components::Transform>(entity2, transform2);
    // std::cout << vao << std::endl;

    // Core::VertexBuffer *vba = new Core::VertexBuffer();
    // vba->GenBuffer(data, 4);
    // Core::ElementBuffer *ebo = new Core::ElementBuffer();
    // ebo->GenBuffer(6, indices);
    // vao->SetVertexDataID(ebo->GetIndex(), vba->GetIndex());
    // mRender->SetVertexArray(vao);
    // glCheckError();
}

} // namespace Managers