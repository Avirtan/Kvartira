#pragma once

#include "ecs/Component.h"
#include "render/shader/Shader.h"
#include "render/vertexArray/VertexArray.h"
#include <memory>

namespace Components {
struct MeshComponent2D : public Ecs::Component {
public:
  MeshComponent2D(std::unique_ptr<Core::VertexArray> &&vao,
                std::shared_ptr<Core::Shader> shader)
      : VertexArray(std::move(vao)), Shader(shader) {}
  ~MeshComponent2D() {}

  std::unique_ptr<Core::VertexArray> VertexArray;
  std::shared_ptr<Core::Shader> Shader;
};
} // namespace Components