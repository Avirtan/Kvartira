#pragma once

#include "ecs/Component.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <memory>

namespace Components {
struct Transform : public Ecs::Component {
public:
  Transform() {
    Matrix = glm::mat4(1.0f);
  }
  ~Transform() {}

//   std::unique_ptr<Core::VertexArray> VertexArray;
  glm::mat4 Matrix;
};
} // namespace Components