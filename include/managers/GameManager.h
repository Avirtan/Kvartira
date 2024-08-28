#pragma once
// #define GLEW_NO_GLU
#include "./SDL.h"
#include "ecs/World.h"
#include "services/InputService.h"
#include <SDL_events.h>
#include <memory>
#include <vector>

namespace Managers {

class GameManager {
  public:
    GameManager();
    bool Init();
    void Run();

    ~GameManager() {}

  private:
    void ProcessInput();
    void UpdateGame();
    void CreateSystem();
    void CreateService();

    void LoadData();

    bool mIsRunning;

    Uint64 mTicksCount;

    SDL_Window *m_Window;
    SDL_GLContext m_Context;

    std::shared_ptr<CoreService::InputService> m_InputeService;
    std::shared_ptr<Ecs::World> m_World;
};
} // namespace Managers