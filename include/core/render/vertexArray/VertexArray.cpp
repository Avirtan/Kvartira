#include "./VertexArray.h"
#include <cstddef>

namespace Core {
VertexArray::VertexArray() {
  glGenVertexArrays(1, &mIndexArray);
  glBindVertexArray(mIndexArray);
}

void VertexArray::SetVertexBuffer(const VertexAttributeData *data, int count) {
  glGenBuffers(1, &mIndexVertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, mIndexVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, count * sizeof(VertexAttributeData), data, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), reinterpret_cast<void *>(offsetof(VertexAttributeData, Position)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), reinterpret_cast<void *>(offsetof(VertexAttributeData, Normal)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributeData), reinterpret_cast<void *>(offsetof(VertexAttributeData, Texture)));
}

void VertexArray::SetElementBuffer(const void *indices, int count, int sizeElement) {
  glGenBuffers(1, &mIndexElementBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexElementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeElement, indices, GL_STATIC_DRAW);
  m_CountIndex = count * 3;
}

void VertexArray::SetActive() { glBindVertexArray(mIndexArray); }

VertexArray::~VertexArray() {
  glDeleteBuffers(1, &mIndexVertexBuffer);
  glDeleteBuffers(1, &mIndexElementBuffer);
  glDeleteVertexArrays(1, &mIndexArray);
}

} // namespace Core