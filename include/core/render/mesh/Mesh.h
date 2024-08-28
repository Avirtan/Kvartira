#pragma once
#include "render/shader/Shader.h"
#include "render/texture/Texture.h"
#include "render/vertexArray/VertexArray.h"
#include <memory>
#include <vector>

namespace Core {
class Mesh {
  public:
    Mesh(std::shared_ptr<Core::VertexArray> &array, std::vector<std::shared_ptr<Core::Texture>> &textures, std::shared_ptr<Core::Shader> &shader);
    ~Mesh();

    // bool Load(const std::string &fileName);

    std::shared_ptr<VertexArray> &GetVertexArray() { return m_VertexArray; }
    std::shared_ptr<Core::Texture> GetTexture(size_t index);
    const std::shared_ptr<Core::Shader> &GetShader() const { return m_Shader; }

    float GetRadius() const { return mRadius; }
    float GetSpecPower() const { return mSpecPower; }

  private:
    std::vector<std::shared_ptr<Core::Texture>> m_Textures;
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<Core::Shader> m_Shader;

    float mRadius;
    float mSpecPower;
};
} // namespace Core