#pragma once

#include "ecs/Component.h"
#include "render/mesh/Mesh.h"
#include "render/shader/Shader.h"
#include "render/texture/Texture.h"
#include "render/vertexArray/VertexArray.h"
#include <memory>

namespace Components {
struct MeshComponent2D : public Ecs::Component {
  public:
    MeshComponent2D(std::shared_ptr<Core::VertexArray> &vao, std::shared_ptr<Core::Shader> shader, std::shared_ptr<Core::Texture> texture = nullptr)
        : VertexArray(vao), Shader(shader), Texture(texture) {}

    MeshComponent2D(std::shared_ptr<Core::Mesh> mesh)
        : VertexArray(mesh->GetVertexArray()), Shader(mesh->GetShader()), Texture(mesh->GetTexture(1)) {}
    ~MeshComponent2D() {}

    std::shared_ptr<Core::VertexArray> VertexArray;
    std::shared_ptr<Core::Shader> Shader;
    std::shared_ptr<Core::Texture> Texture;
};

struct MeshComponent3D : public Ecs::Component {
  public:
    MeshComponent3D(std::shared_ptr<Core::VertexArray> &vao, std::shared_ptr<Core::Shader> shader, std::shared_ptr<Core::Texture> texture = nullptr)
        : VertexArray(vao), Shader(shader), Texture(texture) {}

    MeshComponent3D(std::shared_ptr<Core::Mesh> mesh)
        : VertexArray(mesh->GetVertexArray()), Shader(mesh->GetShader()), Texture(mesh->GetTexture(1)) {}
    ~MeshComponent3D() {}

    std::shared_ptr<Core::VertexArray> VertexArray;
    std::shared_ptr<Core::Shader> Shader;
    std::shared_ptr<Core::Texture> Texture;
};
} // namespace Components