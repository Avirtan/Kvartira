#pragma once
#include "render/vertexArray/VertexArray.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace CoreObject {
class Primitive {
public:
  Primitive() = default;
  ~Primitive() = default;
  virtual void Draw() = 0;
protected:
    Core::VertexArray* mVao;
    glm::mat4 mTransform;
};
} // namespace CoreObject