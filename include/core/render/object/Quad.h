#pragma once

#include "render/object/Primitive.h"
namespace CoreObject {
class Quad : public CoreObject::Primitive {
public:
  Quad();
  ~Quad();
  void Draw() override;
};
} // namespace CoreObject