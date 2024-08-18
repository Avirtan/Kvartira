#include "Mesh.h"

namespace Core {
Mesh::Mesh(const std::shared_ptr<Core::VertexArray> &Vao,
           const std::shared_ptr<Core::Shader> &Shader)
    : m_Vao(Vao), m_Shader(Shader) {}
Mesh::~Mesh() {}

void Mesh::Draw() {
  m_Shader->SetActive();
  m_Vao->SetActive();
}
} // namespace Core