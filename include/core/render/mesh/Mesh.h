#pragma once
#include "render/shader/Shader.h"
#include "render/vertexArray/VertexArray.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

namespace Core {
class Mesh {
public:
  Mesh(const std::shared_ptr<Core::VertexArray> &Vao,
       const std::shared_ptr<Core::Shader> &Shader);
  ~Mesh();
  void Draw();

  std::shared_ptr<Core::Shader> GetShader() const { return m_Shader; }

protected:
  std::shared_ptr<Core::VertexArray> m_Vao;
  std::shared_ptr<Core::Shader> m_Shader;
};
} // namespace Core