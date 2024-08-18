
#include "./GameManager.h"
#include "../core/services/ResourcesService.h"
#include "di/DICore.h"
#include "ecs/Components/MeshComponent.h"
#include "ecs/Components/TransformComponent.h"
#include "ecs/Systems/Mesh/RenderMesh2D.h"
#include "ecs/Systems/MoveSystem.h"
#include "ecs/Systems/Render/RenderInitSystem.h"
#include "ecs/Systems/Render/RenderSwapSystem.h"
#include "ecs/Systems/Render/RenderSystem.h"
#include "ecs/World.h"
#include "render/vertexArray/VertexArray.h"
#include <GL/glew.h>
#include <SDL_events.h>
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <SDL_video.h>
#include <cstddef>
#include <glm/ext.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

namespace Managers {
GameManager::GameManager()
    : mIsRunning(false), mTicksCount(0), mRender(nullptr), m_Window(nullptr) {}

bool GameManager::Init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  int screenWidth = 1024;
  int screenHeight = 768;
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);

  m_Window = SDL_CreateWindow("kvrtira", 0, 0, screenWidth, screenHeight,
                              window_flags);
  if (!m_Window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  // auto isInitRender = InitRender();
  // if (!isInitRender) {
  //   SDL_Log("Failed to init render");
  //   return false;
  // }

  // auto renderComponent = std::make_shared<Components::RenderComponent>(
  //     Components::RenderComponent(m_Window));
  // auto entity = m_World->CreateEntity();
  // m_World->AddComponent<Components::RenderComponent>(entity,
  // renderComponent);

  CreateSystem();

  m_World->InitSystem();

  // mRender = std::make_unique<Core::Render>(Core::Render());
  // mRender->Init(m_Window);

  LoadData();
  // auto shader1 = r2->GetShader(path, path1);
  // std::cout << shader << " " << shader1 << std::endl;

  mIsRunning = true;
  return true;
}

void GameManager::CreateSystem() {
  m_World = std::make_shared<Ecs::World>(Ecs::World());

  auto renderInitSystem = std::make_shared<Systems::RenderInitSystem>(
      Systems::RenderInitSystem(m_Window));
  m_World->AddSystem<Systems::RenderInitSystem>(renderInitSystem);

  auto renderSwapSystem =
      std::make_shared<Systems::RenderSwapSystem>(Systems::RenderSwapSystem());
  m_World->AddSystem<Systems::RenderSwapSystem>(renderSwapSystem);

  auto renderMesh2DSystem =
      std::make_shared<Systems::RenderMesh2D>(Systems::RenderMesh2D());
  m_World->AddSystem<Systems::RenderMesh2D>(renderMesh2DSystem);

  auto moveSystem = std::make_shared<Systems::MoveSystem>();
  m_World->AddSystem<Systems::MoveSystem>(moveSystem);
}

bool GameManager::InitRender() {
  // SDL_GLprofile profile = SDL_GL_CONTEXT_PROFILE_CORE;
  // std::pair<short, short> version = std::pair(4, 3);
  // short sizeColor = 8;
  // short sizeDepth = 24;
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, profile);

  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.first);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.second);

  // SDL_GL_SetAttribute(SDL_GL_RED_SIZE, sizeColor);
  // SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, sizeColor);
  // SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, sizeColor);
  // SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, sizeColor);
  // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, sizeDepth);

  // SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  // m_Context = SDL_GL_CreateContext(m_Window);
  // glewExperimental = GL_TRUE;
  // if (glewInit() != GLEW_OK) {
  //   SDL_Log("Failed to initialize GLEW.");
  //   return false;
  // }
  // glCheckError();

  // const char *glsl_version = "#version 430";
  // IMGUI_CHECKVERSION();
  // ImGui::CreateContext();
  // ImGuiIO &io = ImGui::GetIO();
  // (void)io;
  // io.ConfigFlags |=
  //     ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |=
  //     ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // ImGui::StyleColorsDark();

  // ImGui_ImplSDL2_InitForOpenGL(m_Window, m_Context);
  // ImGui_ImplOpenGL3_Init(glsl_version);

  return true;
}

void GameManager::Run() {
  while (mIsRunning) {
    // if (SDL_GetTicks64() - mTicksCount < 33) {
    //   continue;
    // }
    ProcessInput();
    UpdateGame();
    // GenerateOutput();
  }
}

void GameManager::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // ImGui_ImplSDL2_ProcessEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      mIsRunning = false;
      break;
    case SDL_WINDOWEVENT:
      if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
        std::cout << event.window.data1 << std::endl;
      }
      break;
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
      mIsRunning = false;
    }
  }
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

// void GameManager::GenerateOutput() { mRender->Draw(); }

void GameManager::LoadData() {

  std::string path = "../shaders/Sprite.vert";
  std::string path1 = "../shaders/Sprite.frag";
  auto r = std::make_shared<CoreService::ResourcesService>();
  Core::DiCore::SetObject(r, Core::DIObjects::ResourcesService);
  auto shader = r->GetShader(path, path1);

  std::unique_ptr<Core::VertexArray> vao =
      std::make_unique<Core::VertexArray>();
  Core::VertexAttributeData data[] = {
      {glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0, 0)},
      {glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 0)},
      {glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 1)},
      {glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0, 1)},
  };
  unsigned int indices[] = {0, 1, 2, 2, 3, 0};
  vao->SetVertexBuffer(data, 4);
  vao->SetElementBuffer(indices, 6);

  auto entity = m_World->CreateEntity();
  auto component =
      std::make_shared<Components::MeshComponent2D>(std::move(vao), shader);
  m_World->AddComponent<Components::MeshComponent2D>(entity, component);
  auto transform = std::make_shared<Components::Transform>();

  transform->Matrix = glm::rotate(transform->Matrix, glm::radians(45.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f));
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
  m_World->AddComponent<Components::Transform>(entity, transform);
  // std::cout << entity << std::endl;
  // std::unique_ptr<Core::VertexArray> vao2 =
  //     std::make_unique<Core::VertexArray>();
  // vao2->SetVertexBuffer(data, 4);
  // vao2->SetElementBuffer(indices, 6);

  // auto entity2 = m_World->CreateEntity();
  // auto component2 =
  //     std::make_shared<Components::MeshComponent2D>(std::move(vao2), shader);
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