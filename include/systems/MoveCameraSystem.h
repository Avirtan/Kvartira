#pragma once
#include "di/DICore.h"
#include "ecs/Components/CameraComponent.h"
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cmath>
#include <glm/ext.hpp>

#include "ecs/System.h"
#include "ecs/World.h"
#include "services/InputService.h"
#include <glm/ext/quaternion_common.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <memory>

namespace Systems {
class MoveCameraSystem : public Ecs::System {
  public:
    std::shared_ptr<CoreService::InputService> inputService;
    MoveCameraSystem() {}
    ~MoveCameraSystem() {}

    void Init() override {
        inputService = std::static_pointer_cast<CoreService::InputService>(Core::DiCore::GetObject(Core::DIObjects::InputService));
    }

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::CameraComponent>();
        for (auto ent : pool) {
            auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
            auto position = camera->Position();
            auto forward = camera->Forward();
            auto right = camera->Right();
            if (inputService->GetValueAction(CoreService::InputAction::UP)) {
                position += 10.0f * forward * deltaTime;
            }
            if (inputService->GetValueAction(CoreService::InputAction::DOWN)) {
                position -= 10.0f * forward * deltaTime;
            }
            if (inputService->GetValueAction(CoreService::InputAction::RIGHT)) {
                position += 10.0f * deltaTime * right;
            }
            if (inputService->GetValueAction(CoreService::InputAction::LEFT)) {
                position -= 10.0f * deltaTime * right;
            }
            camera->SetPosition(position);
        }
    }

  private:
};
} // namespace Systems