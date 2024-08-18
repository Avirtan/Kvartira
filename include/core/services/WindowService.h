#pragma once
#include "./SDL.h"

namespace CoreService {
class WindowService {
public:
  WindowService(SDL_Window *window) : m_Window(window) {}
  ~WindowService() {}

  SDL_Window *GetWindow() const { return m_Window; }

private:
  SDL_Window *m_Window;
};
} // namespace CoreService