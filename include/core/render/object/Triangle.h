#pragma once

#include "render/object/Primitive.h"
namespace CoreObject {
class Triangle : public CoreObject::Primitive {
public:
  Triangle();
  ~Triangle();
  void Draw() override;
};
} // namespace CoreObject