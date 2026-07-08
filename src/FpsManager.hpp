#pragma once
#include <SDL3/SDL_stdinc.h>

#define FPS_LOWER_LIMIT		1
#define FPS_UPPER_LIMIT		200

#define FPS_DEFAULT		30

class FPSManager {
    Uint32  frameCount = 0;
    Uint64  baseTicks = 0;
    Uint64  lastTicks = 0;
    float   rateTicks = (1000.0f / static_cast<float>(FPS_DEFAULT));
    Uint32  rate = FPS_DEFAULT;

public:
    FPSManager() = default;

    bool setFramerate(Uint32 rate);
    int getFramerate() const;
    int getFrameCount() const;

    Uint64 framerateDelay();
};
