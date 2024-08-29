#pragma once
#include "di/DICore.h"
#include "ecs/Components/CameraComponent.h"
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cmath>
#include <glm/ext.hpp>
// #define GLM_ENABLE_EXPERIMENTAL

#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include "services/InputService.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/matrix_decompose.hpp>
// #include <glm/gtx/string_cast.hpp>
#include <glm/vec3.hpp>
#include <memory>

namespace Systems {
class MoveSystem : public Ecs::System {
  public:
    std::shared_ptr<CoreService::InputService> inputService;
    MoveSystem() {}
    ~MoveSystem() {}
    float yaw = 90.0f;
    float pitch = 0.0f;
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool isFirst = true;

    void Init() override {
        inputService = std::static_pointer_cast<CoreService::InputService>(Core::DiCore::GetObject(Core::DIObjects::InputService));
        std::cout << inputService->GetMousePosition().x << std::endl;
    }

    float dir = 1.0f;
    void Update(float deltaTime) override {
        auto pool = m_World->GetPoolComponent<Components::Transform>();
        auto pool2 = m_World->GetPoolComponent<Components::CameraComponent>();
        for (auto ent : pool2) {
            auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
            auto position = camera->GetPosition();
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
            // auto speed = 2;
            if (inputService->GetValueAction(CoreService::InputAction::UP)) {
                position += 10.0f * dir * deltaTime;
            }
            if (inputService->GetValueAction(CoreService::InputAction::DOWN)) {
                position -= 10.0f * dir * deltaTime;
            }
            if (inputService->GetValueAction(CoreService::InputAction::RIGHT)) {
                position += glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0))) * 10.0f * deltaTime;
            }
            if (inputService->GetValueAction(CoreService::InputAction::LEFT)) {
                position -= glm::normalize(glm::cross(dir, glm::vec3(0, 1, 0))) * 10.0f * deltaTime;
            }
            // if (position.z > 10) {
            //   dir = 1;
            // }
            // if (position.z <= -10.0f) {
            //   dir = -1;
            // }
            // auto direction = glm::vec3(0.0f, 0.0f, -1.0f);
            // auto target = glm::rotate(direction, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            // position.z += 10 * deltaTime * dir;
            // cos(SDL_GetTicks64() / 20 * deltaTime) * 5;
            // position.x = sin(SDL_GetTicks64() / 20 * deltaTime) * 5;
            // position.y = sin(SDL_GetTicks64() / 20 * deltaTime) * 5;
            // std::cout << position.z << std::endl;
            // camera->SetPositionAndTarget(position, position + dir);
            // camera->SetPosition(position);
            // camera->SetTarget(target);
        }
        // std::cout << "befor move "  <<  SDL_GetTicks64() << std::endl;
        // for (int i = 0; i < std::numeric_limits<int>::max() / 2; i++) {
        //   t = (int)i / 2;
        //   if (t > 2) {
        //     t -= 1;
        //   }
        // }

        //  for (Uint64 i = 0; i < UINT32_MAX / 80; i++) {
        //   t = i / 2;
        //   t -=2;
        // }
        // std::cout << "after move "  <<  SDL_GetTicks64() << std::endl;
        for (auto ent : pool) {
            // auto transform = m_World->GetComponent<Components::Transform>(ent);

            // glm::mat4 transformation;
            // glm::vec3 scale;
            // glm::quat rotation;
            // glm::vec3 translation;
            // glm::vec3 skew;
            // glm::vec4 perspective;

            // glm::decompose(transformation, scale, rotation, translation, skew,
            //                perspective);

            //   transform->Matrix =
            //       glm::translate(transform->Matrix, glm::vec3(0.0, 0.005, 0.0));
            // transform->Matrix =
            //   glm::rotate(transform->Matrix, glm::radians(10.0f * deltaTime * 2.0f)
            //   , glm::vec3(0.5f, 1.0f, 1.0f));
            // auto v = glm::vec3(0.0f, 0.0f, 1.0f);

            // auto MyQuaternion = glm::angleAxis(45.0f, v);
        }
    }

  private:
};
} // namespace Systems