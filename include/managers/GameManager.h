#pragma once
#include "./SDL.h"
#include "actors/Actor.h"
#include "ecs/World.h"
#include "render/Render.h"
#include <SDL_events.h>
#include <memory>
#include <vector>

namespace Managers {

class GameManager {
public:
  GameManager();
  bool Init();
  bool InitRender();
  void Run();

  ~GameManager();

private:
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void CreateSystem();

  void LoadData();

  bool mIsRunning;

  Uint64 mTicksCount;

  std::unique_ptr<Core::Render> mRender;
  SDL_Window *m_Window;
  SDL_GLContext m_Context;

  std::shared_ptr<Ecs::World> m_World;
  // std::vector<std::unique_ptr<Core::Actor>> m_Actors;
};
} // namespace Managers