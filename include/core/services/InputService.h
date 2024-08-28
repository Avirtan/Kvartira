#pragma once

#include "di/DIObject.h"

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <SDL_stdinc.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <unordered_map>

namespace CoreService {
enum class InputAction { UP, DOWN, RIGHT, LEFT, QUIT };
class InputService : public Core::DiObject {
  public:
    InputService() : m_MouseMotion(glm::vec2(0.0f)) { m_MousePosition = glm::vec2(0, 0); }
    ~InputService() {}

    void SetKeyEvent(SDL_KeyboardEvent &event) {
        SDL_Scancode code = event.keysym.scancode;
        auto search = m_KeyBinding.find(code);
        if (search == m_KeyBinding.end()) {
            return;
        }
        auto action = search->second;
        m_KeyValues[action] = event.state;
    }

    void SetBinding(SDL_Scancode key, InputAction action) { m_KeyBinding[key] = action; }

    Uint32 GetValueAction(InputAction action) { return m_KeyValues[action]; }

    const glm::vec2 &GetMouseMotion() const { return m_MouseMotion; }
    const glm::vec2 &GetMousePosition() const { return m_MousePosition; }

    void SetMouseMotion(float x, float y) {
        m_MouseMotion.x = x;
        m_MouseMotion.y = y;
    }

    void SetMouseMotionPosition(float x, float y) {
        m_MousePosition.x += x;
        m_MousePosition.y += y;
    }

    void SetMousePosition(float x, float y) {
        m_MousePosition.x = x;
        m_MousePosition.y = y;
    }

  private:
    std::unordered_map<SDL_Scancode, InputAction> m_KeyBinding{};
    std::unordered_map<InputAction, Uint32> m_KeyValues{};

    glm::vec2 m_MouseMotion;
    glm::vec2 m_MousePosition;
};

} // namespace CoreService