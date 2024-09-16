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
class RotateCameraSystem : public Ecs::System {
  public:
    std::shared_ptr<CoreService::InputService> inputService;
    RotateCameraSystem() : Ecs::System(1) {}
    ~RotateCameraSystem() {}
    float yaw = 90.0f;
    float pitch = 0.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool isFirst = true;

    void Init() override {
        inputService = std::static_pointer_cast<CoreService::InputService>(Core::DiCore::GetObject(Core::DIObjects::InputService));
    }

    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::CameraComponent>();
        for (auto ent : pool) {
            auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
            if (isFirst) {
                lastX = inputService->GetMousePosition().x;
                lastY = inputService->GetMousePosition().y;
                isFirst = false;
            }
            float xpos = inputService->GetMousePosition().x;
            float ypos = inputService->GetMousePosition().y;
            float xoffset = lastX - xpos;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
            auto dir = glm::normalize(glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
                                                -sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
            camera->SetDirection(dir);
        }
    }

  private:
};
} // namespace Systems