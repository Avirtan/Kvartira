#pragma once
#include "actors/Actor.h"

class SpriteA : Core::Actor {
  SpriteA(std::unique_ptr<Core::Mesh> &&mesh) : Core::Actor(std::move(mesh)) {}
  ~SpriteA();
};