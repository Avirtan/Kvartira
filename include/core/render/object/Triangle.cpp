#include "Triangle.h"

namespace CoreObject {
Triangle::Triangle() {
  mVao = new Core::VertexArray();
  // mVao->Bind();
  Core::VertexAttributeData data[] = {
      {glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(0, 0)},
      {glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 0)},
      {glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0, 0, 0.f), glm::vec2(1, 1)},
  };

  // Core::VertexBuffer *vba = new Core::VertexBuffer();
  // vba->GenBuffer(data, 4);
  // Core::ElementBuffer *ebo = new Core::ElementBuffer();
  // GLuint indices[] = {0, 1, 2};
  // ebo->GenBuffer(3, indices);
  // mVao->SetVertexDataID(ebo->GetIndex(), vba->GetIndex());
}

Triangle::~Triangle() { delete mVao; }

void Triangle::Draw() {
  mVao->SetActive();
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

} // namespace CoreObject