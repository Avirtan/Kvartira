#pragma once

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
namespace Core {
struct VertexAttributeData {
  public:
    glm::fvec3 Position;
    glm::fvec3 Normal;
    glm::fvec2 Texture;

    // VertexAttributeData(const glm::vec3 &position, const glm::vec3 &normal,
    //                     const glm::vec2 &texture)
    //     : Position(position), Normal(normal), Texture(texture) {}
};
class VertexArray {
  public:
    VertexArray();
    ~VertexArray();
    void SetVertexBuffer(const VertexAttributeData *data, int count);
    void SetElementBuffer(const void *indices, int count, int sizeElement = sizeof(GLuint));

    std::uint8_t GetCountIndex() const { return m_CountIndex; }
    void SetActive();

  private:
    GLuint mIndexArray;
    GLuint mIndexElementBuffer;
    GLuint mIndexVertexBuffer;
    std::uint8_t m_CountIndex;
};
} // namespace Core