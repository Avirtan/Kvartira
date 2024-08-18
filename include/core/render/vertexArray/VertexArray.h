#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
namespace Core {
struct VertexAttributeData {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 Texture;

  // VertexAttributeData(const glm::vec3 &position, const glm::vec3 &normal,
  //                     const glm::vec2 &texture)
  //     : Position(position), Normal(normal), Texture(texture) {}
};
class VertexArray {
public:
  VertexArray();
  ~VertexArray();
  void SetVertexBuffer(const VertexAttributeData *data, int count);
  void SetElementBuffer(const void *indices, int count,
                        int sizeElement = sizeof(GLuint));

  void SetActive();

private:
  GLuint mIndexArray;
  GLuint mIndexElementBuffer;
  GLuint mIndexVertexBuffer;
};
} // namespace Core