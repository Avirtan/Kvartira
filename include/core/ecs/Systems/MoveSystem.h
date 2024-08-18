#pragma once
#include "ecs/Components/CameraComponent.h"
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cmath>
#include <cstdint>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL

#include "ecs/Components/TransformComponent.h"
#include "ecs/System.h"
#include "ecs/World.h"
#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Systems {
class MoveSystem : public Ecs::System {
public:
  MoveSystem() {}
  ~MoveSystem() {}
  void Init() override {}

  void Update(float deltaTime) override {
    auto pool = m_World->GetPoolComponent<Components::Transform>();
    auto pool2 = m_World->GetPoolComponent<Components::CameraComponent>();
    for (auto ent : pool2) {
      // auto camera = m_World->GetComponent<Components::CameraComponent>(ent);
      // auto position = camera->GetPosition();
      // position.z = cos(SDL_GetTicks64() / 20 * deltaTime) * 5;
      // position.x = sin(SDL_GetTicks64() / 20 * deltaTime) * 5;
      //       position.y = sin(SDL_GetTicks64() / 20 * deltaTime) * 5;

      // camera->SetPositionAndTarget(position);
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