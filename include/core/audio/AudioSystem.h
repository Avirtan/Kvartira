#pragma once
#include "fmod_studio_common.h"
#include <fmod_studio.hpp>

namespace Audio {
class AudioSystem {
  public:
    AudioSystem() { FMOD_STUDIO_STOP_MODE a = FMOD_STUDIO_STOP_IMMEDIATE; }
    ~AudioSystem() {}
};
} // namespace Audio