#pragma once

#include "SDL.h"
#include "ecs/Component.h"
#include <memory>

namespace Components {
struct RenderComponent : public Ecs::Component {
  RenderComponent(SDL_Window *window, SDL_GLContext context)
      : Window(window), Context(context) {}
  SDL_Window *Window;
  SDL_GLContext Context;
};
} // namespace Components